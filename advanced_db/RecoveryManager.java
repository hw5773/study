import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Iterator;
import java.util.LinkedList;

public class RecoveryManager {

	String 				recoveryFileName	=	"D:\\AdvancedDB\\hw2\\recovery.txt";
	DBConnector			db;
	LogManager			lm					=	null;
	LinkedList<String>	undoList			=	new LinkedList<String>();
	LinkedList<String>	redoList			=	new LinkedList<String>();
	LinkedList<Integer>	checkpointNum;
	LinkedList<String>	logMessages			=	new LinkedList<String>();
	
	public RecoveryManager(DBConnector db, LogManager lm){
		this.db = db;
		this.lm = lm;
	}
	
	public void recovery(int lineNum) throws IOException {
		checkpointNum					=	lm.getCheckpointNum();
		int					idx			=	0;
		int					currPoint	=	checkpointNum.get(idx);
		String 				line		=	null;
		String				lst			=	null;
		String[]			trans;
		BufferedWriter		lf;
		BufferedWriter		recoveryFile=	new BufferedWriter(new FileWriter(recoveryFileName, true));
		BufferedReader 		logFile;
		Iterator<String>	iter;
	
		recoveryFile.write("recover " + lineNum);
		recoveryFile.newLine();
		
		logFile		=	new BufferedReader(new InputStreamReader(new FileInputStream(new File(lm.getLogFileName()))));
		
		// find the last checkpoint.
		for (int i = 0; i < currPoint; i++)
		{
			line = logFile.readLine();
		}
		
		// make the initial undo list.
		if (line != null && line.contains("checkpoint")) {
			lst = line.split(" ")[1];
		
			if (lst.contains(","))
				trans = line.split(" ")[1].split(",");
			else {
				trans = new String[1];
				trans[0] = lst;
			}
			for (String t: trans)
				undoList.add(t.trim());
		}
		
		// start redo process from the checkpoint.
		while ((line = logFile.readLine()) != null) {
			String[]	token	=	line.split(",");
			
			for (String s: token)
				s = s.trim();
			
			if (token[0].contains("<"))
			{
				if (token[1].contains("start")) {
					undoList.add(token[0]);
				} else if (token[1].contains("commit") || token[1].contains("abort")) {
					undoList.remove(token[0]);
					redoList.add(token[0]);
				} else {
					String[] toks = token[1].split("\\.");
					String q = "update ";
					String table = toks[0];
					String idValue = toks[1];
					String var = toks[2];
					
					if (token[2].contains("cancle")) {
						continue;
					} 
					
					if (token[2].contains("delete")) {
						q = q + table + " set flag=1 where " + var + "=" + idValue;
					} else {
						q = q + table + " set " + var + "=" + token[3] + " where id=" + idValue;
					}
					db.execSQL(q);
				}
			}
		}
		
		// Write the redo list in the recovery file.
		recoveryFile.write("redo ");
		iter = redoList.iterator();
		if (iter.hasNext())
			recoveryFile.write(iter.next());
		while (iter.hasNext())
			recoveryFile.write(", " + iter.next());
		recoveryFile.newLine();
		
		// Write the undo list in the recovery file.
		recoveryFile.write("undo ");
		iter = undoList.iterator();
		if (iter.hasNext())
			recoveryFile.write(iter.next());
		while (iter.hasNext())
			recoveryFile.write(", " + iter.next());
		recoveryFile.newLine();
		
		// start undo process.
		while (!undoList.isEmpty()) {
			logFile.close();
			logFile = new BufferedReader(new InputStreamReader(new FileInputStream(new File(lm.getLogFileName()))));
			
			int curr = 0, next = 0;
			
			if (checkpointNum.size()==1) {
				curr = 0;
				next = 0;
				
				while ((line = logFile.readLine()) != null) {
					logMessages.addFirst(line);
					System.out.println("added: " + line);
				}
				
			} else {
				curr++;
				next = curr - 1;
				
				for (int i=0; i<checkpointNum.get(curr); i++) {
					line = logFile.readLine();
				}
				
				for (int i=checkpointNum.get(curr); i<checkpointNum.get(next); i++) {
					line = logFile.readLine();
					logMessages.addFirst(line);
					System.out.println("added: " + line);
				}
			}
			
			iter = logMessages.iterator();
			
			while (iter.hasNext()) {
				line = iter.next();
				String[] token = line.split(",");
				
				for (String s: token)
					s = s.trim();
				
				if (undoList.contains(token[0].trim()))
				{
					if (token[1].contains("start")) {
						undoList.remove(token[0]);
					} else if (token[1].contains("commit") || token[1].contains("abort")) {
						continue;
					} else {
						String[] tok = token[1].split("\\.");
						String q = "update ";
						String table = tok[0];
						String idValue = tok[1];
						String var = tok[2];
						String logMessage = token[0].trim() + ", " + token[1].trim() + ", ";
						
						if (token[2].contains("cancel")) {
							continue;
						} 
						
						if (token[2].contains("delete")) {
							q = q + table + " set flag=0 where " + var + "=" + idValue;
							logMessage = logMessage + "cancel";
						} else {
							q = q + table + " set " + var + "=" + token[2] + " where id=" + idValue;
							logMessage = logMessage + token[2].trim();
						}
						db.execSQL(q);
						lm.writeLog(logMessage);
					}
				}
			}
		}
		
		logFile.close();
		
		
		
		recoveryFile.flush();
		recoveryFile.close();
		
		lf = new BufferedWriter(new FileWriter(lm.getLogFileName(), true));
		lf.write("checkpoint");
		lf.newLine();
		lm.setCheckpointNum(lineNum);
		lf.write("recover " + lineNum);
		lf.newLine();
		lf.flush();
		lf.close();
	}
	
	// Rollback
	public void rollback(String id) {
		LinkedList<String> 	logMessages	=	new LinkedList<String>();
		Iterator<String>	iter;
		try {
			String 	line				=	null;
			BufferedReader logFile		=	new BufferedReader(new FileReader(lm.getLogFileName()));
			
			while ((line = logFile.readLine()) != null) {
				if (line.contains(id))
					logMessages.addFirst(line);
			}
			
			iter = logMessages.iterator();
			
			while (iter.hasNext()) {
				// TODO: parses and executes the messages
			}
			
			logFile.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
