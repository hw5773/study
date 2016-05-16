import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Iterator;
import java.util.LinkedList;

public class TransactionProcessor {

	Project2				prj;
	DBConnector				db;
	LogManager				lm;
	RecoveryManager			rm;
	LinkedList<Transaction>	activeTransactions	=	null;
	private String			transactionFileName = 	null;
	private String			searchFileName		=	null;
	
	private int				lineNum 			= 	0;
	private BufferedReader	transactionFile;
	
	public TransactionProcessor(Project2 prj, String transactionFileName) throws IOException {
		this.prj					=	prj;
		this.db						=	prj.getDBConnector();
		this.lm						=	prj.getLogManager();
		this.rm						=	prj.getRecoveryManager();
		this.activeTransactions		=	new LinkedList<Transaction>();
		///// Need to be modified.
		this.transactionFileName	= 	transactionFileName;
		this.searchFileName			=	"search.txt";

		try {
			transactionFile			= 	new BufferedReader(new InputStreamReader(new FileInputStream(new File(this.transactionFileName))));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	public void execute() throws IOException {
		String	line;
		String	keyword = "";
		String	command = "";
		int vacant = 0;
		
		while ((line = transactionFile.readLine()) != null) {
			lineNum++;
			System.out.println(lineNum + ": " + line);
			vacant = line.indexOf(" ");
			Transaction	trans = null;
			if (line.contains("<") && vacant > 0) {
				keyword = line.substring(0, vacant).trim();
				command = line.substring(vacant).trim();
				
				trans = isActive(keyword);
							
				if (trans == null) {
					trans = new Transaction(db, keyword, lm);
					newTransaction(trans);
					trans.executeQuery(command);
				} else if (trans != null) {
					if (command.contains("UPDATE") || command.contains("DELETE")) {
						trans.executeQuery(command);
					} else if (command.contains("commit")) {
						commitTransaction(keyword);
					} else if (command.contains("rollback")) {
						abortTransaction(keyword);
					}
				}
			} else {
				keyword = line.trim();
			}
			
			if (keyword.contains("checkpoint")) {
				lm.checkpoint(activeTransactions);
			}else if (keyword.contains("system")) {
				rm.recovery(lineNum);
			} else if (keyword.contains("search")) {
				command = line.substring(vacant).trim();
				new Rank(prj, command).writeRank(searchFileName, lineNum);
			}
		}

		transactionFile.close();
	}
	
	private Transaction isActive(String id) {
		Iterator<Transaction>	iter	=	activeTransactions.descendingIterator();
		Transaction				trans;
		
		while (iter.hasNext())
		{
			if ((trans = iter.next()).getID().equals(id)) {
				return trans;
			}
		}
		
		return null;
	}
	
	private void newTransaction(Transaction t) {
		String logMessage = t.getID() + ", start";
		lm.writeLog(logMessage);
		activeTransactions.add(t);
		printActiveTransactions();
	}
	
	private void commitTransaction(String id) {
		Iterator<Transaction>	iter		=	activeTransactions.descendingIterator();
		Transaction				trans;
		String					logMessage;
		
		while (iter.hasNext())
		{
			if ((trans = iter.next()).getID().equals(id))
			{
				logMessage = trans.getID() + ", commit";
				lm.writeLog(logMessage);
				activeTransactions.remove(trans);
				break;
			}
		}
		
		printActiveTransactions();
	}
	
	public void rollback(String id) {
		
	}
	
	private void abortTransaction(String id) {
		Iterator<Transaction>	iter	=	activeTransactions.descendingIterator();
		Transaction				trans;
		String					logMessage;
		
		while (iter.hasNext())
		{
			if ((trans = iter.next()).getID().equals(id))
			{	
				logMessage = trans.getID() + ", abort";
				lm.writeLog(logMessage);
				activeTransactions.remove(trans);
				break;
			}
		}
		
		printActiveTransactions();
	}
	
	public LinkedList<Transaction> getActiveTransactions() {
		return activeTransactions;
	}
	
	public void printActiveTransactions() {
		Iterator<Transaction> iter = activeTransactions.descendingIterator();
		System.out.print("current active transactions: ");
		
		while(iter.hasNext()) {
			System.out.print(iter.next().getID() + ", ");
		}
		System.out.print("\r\r");
		System.out.println();
	}
}
