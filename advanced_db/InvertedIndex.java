import java.io.StringReader;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

import edu.stanford.nlp.ling.CoreLabel;
import edu.stanford.nlp.process.CoreLabelTokenFactory;
import edu.stanford.nlp.process.PTBTokenizer;

public class InvertedIndex {
	DBConnector db;
	ArrayList<String> stopwords;
	Stopwords sw;
	ResultSet rs;
	
	public InvertedIndex(DBConnector db) {
		this.db = db;
		sw = new Stopwords();
		stopwords = sw.getStopwords();
		createTable();
		makeInvertedIndex();
	}
	
	// Creating the InvertedIndex table
	private void createTable() {
		System.out.println("Creating the InvertedIndex table...");
		db.execSQL("DROP TABLE IF EXISTS InvertedIndex");
		db.execSQL("CREATE TABLE InvertedIndex("
					+ "term varchar(1000)," 					// term
					+ "id int(11),"								// document ID
					+ "tf double(10,10) zerofill"				// term frequency in the document
					+ ")");
		System.out.println("Creating the InvertedIndex table complete");
	}
	
	// Checking whether the term is the number to remove the stopwords
	boolean isNum(String s) {
		try {
			Integer.parseInt(s);
			return true;
		} catch (NumberFormatException e) {
			return false;
		}
	}
	
	// Making the InvertedIndex table
	void makeInvertedIndex() {
		System.out.println("Making the InvertedIndex table with the Term Frequency...");
		try {
			rs = db.query("select id, text from wiki"); // getting the document id and the contents from the wiki table
			rs.last();
			int numOfDocu						= rs.getRow();
			rs.beforeFirst();
			int numOfDone 						= 0;
			
			// Process by each document.
			while (rs.next()) {
				TFNode head 					= new TFNode("");
				int id 							= rs.getInt("id"); // document id
				String txt 						= rs.getString("text"); // the content of the document
			    PTBTokenizer<CoreLabel> ptbt 	= new PTBTokenizer<CoreLabel>(new StringReader(txt),
			         new CoreLabelTokenFactory(), ""); // parsing it using PTBTokenizer
			    
			    // The terms in the document.
			    double numOfTerms = 0;
			    while (ptbt.hasNext()) {
			    	String term = ptbt.next().toString();
			    	numOfTerms++;
			    	if (stopwords.contains(term.toLowerCase()) || isNum(term)) { // skip if the token is the stopwords.
			    		continue;
			    	} else {
			    		TFNode tmp = null;
			    		if ((tmp = contains(head, term)) != null) {
			    			tmp.addOne();
			    		} else {
			    			head = addTFNode(head, term);
			    		}
			    	}
			    }
			    
			    insertToTable(id, head, numOfTerms); // insert the information of the document into the invertedIndex table.
			    numOfDone++;
			    System.out.print("\r" + (numOfDone*100.0/numOfDocu) + "% is complete");
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		System.out.println("\rMaking the InvertedIndex table with the Term Frequency complete");
	}
	
	// Insert the records into the table InvertedIndex
	public void insertToTable(int id, TFNode tf, double nd) {
		if (tf != null) {
			do {
				tf = tf.getNext();
				String sql = "insert into InvertedIndex(term, id, tf) values(\"" + tf.getTerm() 
							+ "\", \"" + id + "\", \"" + Math.log((double)tf.getFreq()/nd + 1) + "\")";
				db.execSQL(sql);
			} while (tf.hasNext());
		}
	}
	
	// add new TFNode
	public TFNode addTFNode(TFNode tf, String term) {
		TFNode tmp = new TFNode(term);
		tmp.setNext(tf);
		
		return tmp;
	}
	
	// retrieve the term in the TFNode list and return the pointer to the node in question
	public TFNode contains(TFNode tf, String term) {
		
		while (tf.hasNext()) {
			tf = tf.getNext();
			if (tf.getTerm().equals(term)) {
				return tf;
			}
		}
		
		return null;
	}
}