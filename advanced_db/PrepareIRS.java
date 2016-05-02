import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Hashtable;

public class PrepareIRS {

	ResultSet rs;
	DBConnector db;
	Hashtable<Integer, String> tTable;
	TF_IDF tf;
	PageRank pr;
	
	public PrepareIRS(DBConnector db) {
		this.db = db;
		makeTitle();
		//new InvertedIndex(db);
		tf	= new TF_IDF(db);
		pr	= new PageRank(db);
	}
	
	// Getting the TF_IDF instance
	public TF_IDF getTF_IDF() {
		return tf;
	}
	
	// Getting the PageRank instance
	public PageRank getPageRank() {
		return pr;
	}
	
	// Making the title table
	private void makeTitle() {
		
		try {
			String sql 	= "select id, title from wiki";
			rs			= db.query(sql);
			int id;
			String title;
			int numOfRows;
			rs.last();
			numOfRows = rs.getRow();
			rs.beforeFirst();
			tTable		= new Hashtable<Integer, String>(numOfRows);
			
			while (rs.next()) {
				id 		= rs.getInt("id");
				title 	= rs.getString("title");
				tTable.put(id, title);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	
	// Getting the title hashtable
	public Hashtable<Integer, String> getTitleTable() {
		return tTable;
	}
}
