// Term Frequency Node in the document 
class TFNode {
	private String term;
	private int freq			= 0;
	private TFNode next			= null;
	private static int size		= 0;
	
	public TFNode(String term) {
		this.term = term;
		this.freq = 1;
		size += 1;
	}
	
	// get the term
	public String getTerm() {
		return term;
	}
	
	// get the TF
	public int getFreq() {
		return freq;
	}
	
	// get the next node
	public TFNode getNext() {
		return next;
	}
	
	public void setNext(TFNode tf) {
		next = tf;
	}
	
	// add one to the frequency
	public void addOne() {
		freq = freq + 1;
	}
	
	public int getSize() {
		return size;
	}
	
	// return to 0 for new linked list.
	public void zero() {
		size = 0;
	}
	
	// return whether this node has the next.
	public boolean hasNext() {
		if (next == null) {
			return false;
		}
		return true;
	}
}