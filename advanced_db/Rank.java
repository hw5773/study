import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.ResultSet;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;


public class Rank {

	DBConnector db;
	String[] keywords;
	static final int MAXNUM					= 10;
	ArrayList<Entry<Integer, Double>> rankResult;
	ResultSet rs;
	Hashtable<Integer, String> tTable;		// The title table from the document ID to the title
	Hashtable<Integer, Double> tfTable;		// The TF table from the document ID to the TF
	Hashtable<Integer, Double> prTable;		// The PageRank table from the document ID to the PageRank
	
	public Rank(Project2 prj, String keywords) {
		this.db = prj.getDBConnector();
		this.keywords 	= keywords.split(" ");
		
		tfTable 		= prj.getIRS().getTF_IDF().getTF_IDFTable(this.keywords);	// TF-IDF table from the document ID to the value
		prTable			= prj.getIRS().getPageRank().getPageRankTable();				// PR table from the document ID to the value
		tTable			= prj.getIRS().getTitleTable();
		
		makeRank();
	}
	

	private void makeRank() {
		Enumeration<Integer> docuIDs		= tfTable.keys();		// The list of the document ID

		// The result table TF-IDF * PageRank
		Hashtable<Integer, Double> resTable = new Hashtable<Integer, Double>(tfTable.size());
		
		while (docuIDs.hasMoreElements()) {
			Integer key = docuIDs.nextElement();
			Double val = tfTable.get(key) * prTable.get(key);
			resTable.put(key, val);
		}
		
		sortByValue(resTable);
	}
	
	// Sorting the result by values
	public void sortByValue(Hashtable<Integer, Double> h) {
		rankResult = new ArrayList<Entry<Integer, Double>>(h.entrySet());
		Collections.sort((ArrayList<Entry<Integer, Double>>) rankResult, new MyComparator());
	}
	
	// Displaying the MAXNUM (10) results for the keywords
	public void displayRank() {
		Iterator<Entry<Integer, Double>> i = rankResult.iterator();
		int cnt = 0, key;
		String result;
		DecimalFormat df	= new DecimalFormat("0.00000000");
		
		System.out.println("");
		while (i.hasNext() && cnt < MAXNUM) {
			cnt++;
			Entry<Integer, Double> e = (Entry<Integer, Double>)(i.next());
			key = e.getKey();
			result = key + ", " + tTable.get(key) + ", " + df.format(tfTable.get(key))
					+ ", " + df.format(prTable.get(key));
			System.out.println(result);
		}
		
		if (cnt == 0) {
			System.out.println("No documents for this keywords");
		} else {
			System.out.println(cnt + " results are displayed");
		}
		System.out.println("");
	}
	
	// Write the rank in the search file.
	public void writeRank(String searchFileName, int lineNum) {
		BufferedWriter	searchFile = null;
		Iterator<Entry<Integer, Double>> i = rankResult.iterator();
		int cnt = 0, key, idx;
		String result, queryInfo;
		DecimalFormat df	= new DecimalFormat("0.00000000");
		
		try {
			searchFile	=	new BufferedWriter(new FileWriter(searchFileName, true));	
			searchFile.write("search " + lineNum);
			searchFile.newLine();
			queryInfo = "query ";
			
			for (idx=0; idx<keywords.length; idx++) {
				queryInfo += keywords[idx];
				queryInfo += ", ";
			}
			
			idx = queryInfo.lastIndexOf(',');
			searchFile.write(queryInfo.substring(0, idx));
			searchFile.newLine();
				
			while (i.hasNext() && cnt < MAXNUM) {
				cnt++;
				Entry<Integer, Double> e = (Entry<Integer, Double>)(i.next());
				key = e.getKey();
				result = key + ", " + tTable.get(key) + ", " + df.format(tfTable.get(key))
					+ ", " + df.format(prTable.get(key));
				searchFile.write(result);
				searchFile.newLine();
			}
			searchFile.flush();
			searchFile.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}

// The comparator for the Hashtable
class MyComparator implements Comparator<Map.Entry<Integer, Double>> {
	public int compare(Map.Entry<Integer,Double> obj1, Map.Entry<Integer, Double> obj2) {
		int result = 0;
				
		Double value1 = obj1.getValue();
		Double value2 = obj2.getValue();
		
		if(value1.compareTo(value2) == 0) {
			Integer key1 = obj1.getKey();
			Integer key2 = obj2.getKey();
			
			if (key1 > key2)
				result = 1;
			else 
				result = -1;
		} else {
			if (value1 > value2)
				result = -1;
			else
				result = 1;
		}
		
		return result;
	}
}