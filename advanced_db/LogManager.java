import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;

public class LogManager {
	
	String 						logFileName		=	"prj2.log";
	ArrayList<String>			logBuffer		=	new ArrayList<String>();
	BufferedWriter				logFile			=	null;
	TransactionProcessor		tp				=	null;
	static LinkedList<Integer>	checkpointNum	=	new LinkedList<Integer>();
	
	public LogManager() {
		checkpointNum.addFirst(0);
	}
	
	public LogManager(String logFileName) {
		checkpointNum.addFirst(0);
		this.logFileName		=	logFileName;
	}
	
	// Writing the log into the log file.
	public void writeLog(String logMessage) {
		logBuffer.add(logMessage);
		write();
	}
	
	// Checkpoint
	public void checkpoint(LinkedList<Transaction> activeTransactions) {
		String 					logMessage	=	"checkpoint ";
		Iterator<Transaction> 	iter		=	activeTransactions.iterator();
		int						idx;
		
		while (iter.hasNext()) {
			logMessage += iter.next().getID();
			logMessage += ", ";
		}
		idx = logMessage.lastIndexOf(',');
		logBuffer.add(logMessage.substring(0, idx));
		write();
		
		try {
			int		lineNum				=	0;
			String 	line				=	null;
			BufferedReader logFile		=	new BufferedReader(new FileReader(logFileName));
			
			while ((line = logFile.readLine()) != null) {
				lineNum++;
				if (line.contains("checkpoint"))
					checkpointNum.addFirst(lineNum);
			}
			
			logFile.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	// Write to the file from the buffer.
	private void write() {
		try {
			logFile		=	new BufferedWriter(new FileWriter(logFileName, true));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		Iterator<String> iter;
		String logMessage;
		iter = logBuffer.iterator();
		
		while (iter.hasNext()) {
			logMessage = iter.next();
			System.out.println(logMessage);
			try {
				logFile.write(logMessage);
				logFile.newLine();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		try {
			logFile.flush();
			logFile.close();
		} catch(IOException e) {
			e.printStackTrace();
		}
		logBuffer.clear();
	}
	
	// Getting the line number of the checkpoint in the log file.
	public LinkedList<Integer> getCheckpointNum() {
		return checkpointNum;
	}
	
	// Add checkpoint number.
	public void setCheckpointNum(int num) {
		checkpointNum.add(num);
	}
	
	// Clear the log file.
	public void closeLogFile() {
		write();
		try {
			logFile.flush();
			logFile.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public String getLogFileName() {
		return logFileName;
	}
}
