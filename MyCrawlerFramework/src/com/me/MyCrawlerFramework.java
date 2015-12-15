package com.me;

import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import us.codecraft.webmagic.Page;
import us.codecraft.webmagic.Request;
import us.codecraft.webmagic.Site;
import us.codecraft.webmagic.Spider;
import us.codecraft.webmagic.pipeline.ConsolePipeline;
import us.codecraft.webmagic.processor.PageProcessor;

public class MyCrawlerFramework implements PageProcessor{

	//private static final String REGEX = "^(https?://).+\\.(html|htm)$";
	public static Map visitedUrl= new HashMap<String,Integer>();
	public static Map<Integer , Set<Integer>> degree = new HashMap<Integer , Set<Integer>>();
	public static String startUrl = "http://www.sohu.com";
	public static int count = 0;

		// TODO Auto-generated method stub
	private Site site = Site.me()
	           .addStartUrl(startUrl);
	public static void main(String[] args) {
    	List<String> startList = new ArrayList<String>();
    	startList.add(startUrl);
    	MyCrawlerFramework.addVisitedUrl(startList);
        Spider.create(new MyCrawlerFramework())
        .thread(20)
             .pipeline(new ConsolePipeline()).run();
        
//        try {
//        	StringBuilder st = new StringBuilder();
//			FileWriter fw = new FileWriter("f:\\visitedUrl-v2.txt",true);
//			Iterator it =  visitedUrl.entrySet().iterator();
//			while(it.hasNext()){
//				Map.Entry entry = (Map.Entry)it.next(); 
//				st.append(entry.getValue()+":\t"+entry.getKey()+"\n");
//			}
//			fw.write(st.toString());
//			fw.close();
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
    }
        @Override
        public void process(Page page) {
            List<String> toResuest = new ArrayList<String>();	
        	//System.out.println("当前页："+page.getUrl().toString());
            List<String> links = page.getHtml().links().all();
            
            links = removeDuplate(links);//保证没有重复
            toResuest = addVisitedUrl(links);//没有已被访问的url
            addDegree(links,page.getUrl().toString());
            count+=toResuest.size();
            System.out.println(count);
   
            page.addTargetRequests(toResuest);
//            System.out.println(links.size());
//            for(int i = 0 ;i< 20;i++){
//            	System.out.println(links.get(i)+"   *****");
//           }
            
        }

        @Override
        public Site getSite() {
            return site;

        }

	        
	        public static List removeDuplate(List<String> list){
	        	/*Set<String> tempSet = new HashSet(list);
	        	list.clear();
	        	for(String temp : tempSet){
	        		list.add(temp);
	        	}*/
	        	List temp = new ArrayList<String>();
	        	for(String str : list){
	        		if(!temp.contains(str)){
	        			temp.add(str);
	        		}
	        	}
	        	return temp;
	        }
	        public static List<String> bloomFilter(List<String> list){
	        	//布隆过滤器
	        	return list;
	        }
	        public static List<String> addVisitedUrl(List<String> url) {
	        	List<String> returnList = new ArrayList<String>();
    			FileWriter fw = null;
				try {
					fw = new FileWriter("f:\\visitedUrl-v3.txt",true);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}

	        	for(String str : url){
	        		if(!visitedUrl.containsKey(str)){
	        			visitedUrl.put(str, visitedUrl.size());	     
	        			returnList.add(str);
	        			try {
	    	    				fw.write(visitedUrl.size()+":\t"+str+"\t"+"\n");
	    	    				fw.flush();
//	    	    			}
	    	    		} catch (IOException e) {
	    	    			// TODO Auto-generated catch block
	    	    			e.printStackTrace();
	    	    		}
	        		}
	        	}
				try {
					fw.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

	        	return returnList;
	        }
	        public static synchronized void addDegree(List<String> list , String from){
	        	Integer fromId = (Integer) visitedUrl.get(from);
	        	
	        	for(String tempUrl : list){
	        		Set<Integer> urls = null;
	        		int id = (int) visitedUrl.get(tempUrl);
	        		if(!degree.containsKey(id)){
	        			urls = new HashSet<Integer>();
	        			urls.add(fromId);
	        		}
	        		else{
	        			urls = degree.get(id);
	        			urls.add(fromId);
	        		}
        			degree.put(id, urls);
        			
	        	}
	        	if(count > 160000){
        			 try {
//        				FileWriter fw = new FileWriter("f:\\relationUrl.txt",true);
//        				fw.write(degree.toString()+"\n");
//        				fw.flush();
//        				fw.close();
        				ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("f:\\relationUrl-v3.txt"));
        				oos.writeObject(degree);
        				oos.flush();
        				oos.close();
        				
    	    		} catch (IOException e) {
    	    			// TODO Auto-generated catch block
    	    			e.printStackTrace();
    	    		}finally{
    	    			System.exit(0);
    	    		}
	        	}
	        	
	        }
}
