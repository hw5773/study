import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Hashtable;
import java.util.LinkedList;

public class PageRank {

	DBConnector db;							// the DB context
	double[] P;								// the pageRank value, indicating P in the equation
	double[][] T;							// the T array.
	int N;									// the number of pages, indicating N in the equation
	Hashtable<Integer, Integer> map;		// mapping between the document id and the index
	Hashtable<Integer, Integer> invMap;		// inverse mapping from the index to the document id
	Hashtable<Integer, Double> T_value;		// the value of T from the document id
	Hashtable<Integer, Double> pageRank;	// the result of the pageRank, which maps from the document id to the pageRank value
	final double gamma			= 0.15;
	final double cutoff			= 1.0e-8;
	
	
	public PageRank(DBConnector db) {
		this.db 				= db;
		getLinkTable();
		makePageRank();
	}
	
	// Getting the pageRank table
	public Hashtable<Integer, Double> getPageRankTable() {
		return pageRank;
	}
	
	// Getting the information about the linkage and Making the T array.
	private void getLinkTable() {
		String sql 					= "select distinct id_from, id_to from link order by id_from asc";
		ResultSet from_to 			= db.query(sql);
		sql							= "select distinct id from wiki";
		ResultSet docuList			= db.query(sql);
		
		try {
			docuList.last();
			N						= docuList.getRow();
			docuList.beforeFirst();
			ArrayList<Integer> docu	= new ArrayList<Integer>(N);
			map 					= new Hashtable<Integer, Integer>(N);
			invMap 					= new Hashtable<Integer, Integer>(N);
			int id;
			int num					= 0;
			
			while(docuList.next()) {
				id = docuList.getInt("id");
				docu.add(docuList.getInt("id"));
				map.put(id, num);
				invMap.put(num++,  id);
			}
			
			int prevFrom = 0, Ni = 0, curFrom, curTo;
			LinkedList<Integer> tmp	= new LinkedList<Integer>();
			T 						= new double[N][N];
			
			// Making the map table from the document id to the node which gets the pointer
			while(from_to.next()) {
				curFrom				= from_to.getInt("id_from");
				curTo				= from_to.getInt("id_to");
				
				// If changes to the next node. 
				if (prevFrom != curFrom) {
					if (prevFrom != 0) {
						double t;
						
						if (Ni > 0) {
							t			= 1.0/Ni;
						} else {
							t			= 0.0;
						}
						
						Ni				= 0;
						int node;
					
						while(!tmp.isEmpty()) {
							node 		= tmp.pop();
							if (map.containsKey(node)) {
								T[map.get(prevFrom)][map.get(node)] = t;
							}
						}
					}
					tmp				= new LinkedList<Integer>();
				}
				
				prevFrom = curFrom;
				
				// If this 'from node' doesn't contain in the wiki table.
				if (!docu.contains(curFrom))
					continue;
				else {
					tmp.push(curTo);
					Ni++;
				}
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	
	// making the pageRank table
	private void makePageRank() {
		System.out.println("Calculating the PageRank...");
		P 							= new double[N];
		Arrays.fill(P, 1.0/N);
		double[] tmp 				= P.clone();
		double t 					= 0.0;
		
		do {
			P = tmp.clone();
			for (int j = 0; j < N; j++) {
				t = 0;
				for (int i = 0; i < N; i++) {
					t += T[i][j] * P[i];
				}
				tmp[j] 				= gamma/N + (1 - gamma)*t;
			}
		} while (sumDifference(P, tmp) > cutoff);
		
		pageRank 					= new Hashtable<Integer,Double>(N);
		P = tmp;
		
		for (int i = 0; i < N; i++) {
			pageRank.put(invMap.get(i), P[i]);
		}
		System.out.println("Calculating the PageRank complete");
	}

	// Checking the difference between before and after page making
	private double sumDifference(double[] a, double[] b) {
		double sumDiff				= 0.0;
		
		for (int i = 0; i < a.length; i++) {
			sumDiff				+= Math.abs(a[i] - b[i]);
		}
		
		return sumDiff;
	}
}