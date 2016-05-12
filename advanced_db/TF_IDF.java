import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Hashtable;

public class TF_IDF {

	DBConnector db;
	String[] keywords;
	Hashtable<Integer, Double> rTable;
	
	// The constructor for the TF_IDF
	public TF_IDF(DBConnector db) {
		this.db = db;
	}
	
	// Getting the TF_IDF hash table
	public Hashtable<Integer, Double> getTF_IDFTable(String[] keywords) {
		this.keywords = keywords;
		makeRTable();
		return rTable;
	}

	// Making the result table for the inputted keywords
	private void makeRTable() {
		String sql;
		ResultSet rs;
		Hashtable<String, Double> idfTable	= new Hashtable<String, Double>(keywords.length);
		try {
			// Getting the records of counts for each terms
			sql = "select term, count(*) as nt from (select id, term from InvertedIndex where lower(term)='" + keywords[0].toLowerCase() +"'";
			
			for (String s : keywords) {
				sql = sql + " or lower(term)='" + s.toLowerCase() + "'";
			}
			
			sql = sql + ") A group by term";
			
			rs = db.query(sql);
			int size = 0, nt;
			String term;
			
			// Making the idf table
			while(rs.next()) {
				term 	= rs.getString("term");
				nt		= rs.getInt("nt");
				size	+= nt;
				idfTable.put(term, 1.0/nt);
			}
			
			rTable = new Hashtable<Integer, Double>(size);
			
			// Getting the result records with the case-insensitive keywords
			sql = "select id, term, tf from InvertedIndex where lower(term)='" + keywords[0].toLowerCase() + "'";
			for (String s : keywords) {
				sql = sql + " or lower(term)='" + s.toLowerCase() + "'";
			}
			rs = db.query(sql);
			
			while (rs.next()) {
				int key = rs.getInt("id");
				String t = rs.getString("term");
				double val = rs.getDouble("tf") * idfTable.get(t);
				if (rTable.containsKey(key)) {
					double curr = rTable.get(key);
					rTable.replace(key, curr+val);
				} else {
					rTable.put(key, val);
				}
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
}
