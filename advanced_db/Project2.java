import java.io.IOException;
import java.util.Scanner;

public class Project2 {

	DBConnector					db;
	PrepareIRS					irs;
	static TransactionProcessor	tp;
	LogManager					lm;
	RecoveryManager				rm;
	
	public Project2() {
		System.out.println("Connecting to jdbc:mariadb://147.46.15.238:3306/ADB-2015-21259 ...");
		db 	= 	new DBConnector("147.46.15.238:3306", "ADB-2015-21259", "ADB-2015-21259");
		// Comment the InvertedIndex for reducing the consuming time in debugging.
		irs	= 	new PrepareIRS(db);
		lm	=	new LogManager();
		rm	=	new RecoveryManager(db, lm);
	}
	
	public static void main(String[] args) throws IOException{
		
		Rank r;
		System.out.println("AdvancedDB the second project");
		System.out.println("2015-21259 Hyunwoo Lee");
		System.out.println("Starting the Information Retrieval System");
		Project2 prj = new Project2();
		System.out.println("Input the keywords to retrieve or 'quit'");
		String prompt = "ADB-2015-21259> ";
		
		String keywords;
		Scanner scan = new Scanner(System.in);
		
		// Starting the prompt.
		while (true) {
			String[] inputs;
			System.out.print(prompt);
			keywords = scan.nextLine();
			inputs = keywords.split(" ");
			
			// if 'quit' is inputed then exit the program.
			if (keywords.equals("quit")) {
				System.out.println("Quitting the Information Retrieval System.");
				System.out.println("Thank you.");
				scan.close();
				System.exit(0);
			} else if (inputs[0].equals("-run")) {
				setTransactionProcessor(new TransactionProcessor(prj, inputs[1]));
				tp.execute();
			} else if (keywords.equals("")) {
				System.out.println("No keywords are inputted. Please try again.");
			} else {
				r = new Rank(prj, keywords);
				r.displayRank();
			}
		}
	}
	
	// Getting the DBConnector instance
	public DBConnector getDBConnector() {
		return db;
	}
	
	// Getting the PrepareIRS instance
	public PrepareIRS getIRS() {
		return irs;
	}
	
	// Getting the Transaction Processor
	public TransactionProcessor getTransactionProcessor() {
		return tp;
	}
	
	// Getting the LogManager
	public LogManager getLogManager() {
		return lm;
	}
	
	// Getting the RecoveryManager
	public RecoveryManager getRecoveryManager() {
		return rm;
	}
	
	// Setting the TransactionProcessor
	public static void setTransactionProcessor(TransactionProcessor t) {
		tp = t;
	}
}
