import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.DatabaseMetaData;

public class DBConnector {
	
	String driver		= "org.mariadb.jdbc.Driver";
	String url			= "jdbc:mariadb://147.46.15.238:3306/ADB-2015-21259";
	String uId			= "ADB-2015-21259";
	String uPwd			= "ADB-2015-21259";
	
	Connection 			con;
	Statement			stmt;
	PreparedStatement	pstmt;
	DatabaseMetaData	meta;
	
	public DBConnector() {
		initDB();
	}
	
	public DBConnector(String url, String uId, String uPwd) {
		this.url = "jdbc:mariadb://" + url + "/" + uId;
		this.uId = uId;
		this.uPwd = uPwd;
		
		initDB();
	}
	
	// Initialize the Database Configuration
	private void initDB() {
		
		try {
			Class.forName(driver);
			con = DriverManager.getConnection(url, uId, uPwd);
					
			if ( con != null ) {
				System.out.println("Connection Success!");
			}
		} catch (ClassNotFoundException e) {
			System.out.println("Loading the driver error.");
		} catch (SQLException e) {
			System.out.println("Connection Failed.");
		}
	}

	// Execute the SQL statement without any results
	public void execSQL(String sql) {
		try {
			stmt		= con.createStatement();
			stmt.executeUpdate(sql);
		} catch (SQLException e) {
			e.printStackTrace();
		} 
	}
	
	// Execute the SQL statement with the results
	public ResultSet query(String sql) {
		ResultSet rs 	= null;
				
		try {
			pstmt			= con.prepareStatement(sql);
			rs				= pstmt.executeQuery(sql);
		} catch (SQLException e) { 
			e.printStackTrace();
		}
		
		return rs;
	}
}