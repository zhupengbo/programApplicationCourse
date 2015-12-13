import java.io.BufferedOutputStream;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Map;
import java.util.Set;

public class Test {
	public static int a = 1;
	public int b = 2;
	static {
		System.out.println(a);
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try {
			BufferedWriter bw = new BufferedWriter(new FileWriter("f:\\object2int.txt",true));
			ObjectInputStream ois = new ObjectInputStream(new FileInputStream("f:\\relationUrl.txt"));
			Map<Integer,Set<Integer>> map = (Map<Integer, Set<Integer>>) ois.readObject();
			
			for(Map.Entry<Integer, Set<Integer>> entry : map.entrySet()){
				//System.out.println(map.get(i));
				bw.write(entry.getKey()+":\t"+entry.getValue()+"\n");
			}
			bw.flush();
			ois.close();
			bw.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
