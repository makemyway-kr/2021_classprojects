package sp21_simulator;

// instruction에 따라 동작을 수행하는 메소드를 정의하는 클래스

public class InstLuncher {
    ResourceManager rMgr;

    public InstLuncher(ResourceManager resourceManager) {
        this.rMgr = resourceManager;
    }

    public void add(int m)
    {
    	rMgr.setRegister(0,rMgr.register[0]+=m);
    }
    public void ADDF(int m)
    {
    	
    }
    public void ADDR(int r1,int r2)
    {
    	
    }
    public void CLEAR(int r1)
    {
    	
    }
    public void COMP(int m)
    {
    	
    }
    public void COMPR(int r1, int r2)
    {
    	
    }
    public void DIV(int m)
    {
    	
    }
    public void DIVR(int r1, int r2)
    {
    	
    }
    public void J(int m)
    {
    	
    }
    public void JEQ(int m)
    {
    	
    }
    public void JLT(int m)
    {
    	
    }
    public void JSUB(int m)
    {
    	
    }
    public void LDA(int m) 
    {
    	
    }
    }