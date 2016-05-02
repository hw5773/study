import java.sql.ResultSet;
import java.sql.SQLException;

public class Transaction {
	DBConnector			db			= 	null;
	LogManager			lm			=	null;
	private String 		id 			= 	"";
	private String[]	keywords	=	null;
	private	String		command		=	"";
	
	public Transaction(DBConnector db, String id, LogManager lm) {
		this.db 		= 	db;
		this.id 		= 	id;
		this.lm			=	lm;
	}
	
	public void executeQuery(String query) {
		keywords					=	query.split(" ");
		command						=	keywords[0];
		String		table			= 	"";
		String		variable		=	"";
		String		oldValue		=	"";
		String		newValue		=	"";
		String		id				=	"";
		String		idValue			=	"";
		String		selectClause	=	"";
		String		logMessage		=	"";
		
		ResultSet	rs				= 	null;
		int			beginIndex		=	0;
		int			endIndex		=	0;
		String[]	whereClause		=	null;
		
		if (command.equals("UPDATE")) {
			table					=	keywords[1];
			variable				=	keywords[3];
			beginIndex				=	query.indexOf("=");
			endIndex				=	query.indexOf("WHERE");
			newValue				=	query.substring(beginIndex+1, endIndex).trim();
			whereClause				=	query.substring(endIndex).split(" ");
			id						=	whereClause[1];
			beginIndex				=	whereClause[3].indexOf("'");
			endIndex				=	whereClause[3].lastIndexOf("'");
			idValue					= 	whereClause[3].substring(beginIndex+1,  endIndex);
			
			try {
				selectClause		=	"select " + variable + " from " + table + " where " + id + " = '" + idValue + "'";
				rs					=	db.query(selectClause);
				if (rs.next()) {
					oldValue		=	rs.getString(variable);
				} else {
					oldValue		= 	"none";
				}
				
			} catch (SQLException e) {
				e.printStackTrace();
			}
			logMessage				= 	this.id + ", " + table + "." + idValue + "." + variable + ", " + oldValue + ", " + newValue;
		} else if (command.equals("DELETE")) {
			table					=	keywords[2];
			variable				=	keywords[4];
			newValue				=	"none";
			id						=	keywords[4];
			beginIndex				=	keywords[6].indexOf("'");
			endIndex				=	keywords[6].lastIndexOf("'");
			idValue					=	keywords[6].substring(beginIndex+1,  endIndex);
			logMessage				= 	this.id + ", " + table + "." + idValue + "." + variable + ", delete";
			query					=	"update " + table + " set flag=1 where " + id + "=" + idValue;
		}
		
		lm.writeLog(logMessage);
		db.execSQL(query);
		
	}
	
	public String getID() {
		return id;
	}
}
