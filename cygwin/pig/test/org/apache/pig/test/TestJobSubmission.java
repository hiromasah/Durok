/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.pig.test;

import java.io.File;
import java.util.Iterator;
import java.util.Random;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.mapred.jobcontrol.Job;
import org.apache.hadoop.mapred.jobcontrol.JobControl;
import org.apache.pig.ExecType;
import org.apache.pig.backend.executionengine.ExecException;
import org.apache.pig.backend.hadoop.datastorage.ConfigurationUtil;
import org.apache.pig.backend.hadoop.executionengine.HExecutionEngine;
import org.apache.pig.backend.hadoop.executionengine.mapReduceLayer.JobControlCompiler;
import org.apache.pig.backend.hadoop.executionengine.mapReduceLayer.JobCreationException;
import org.apache.pig.backend.hadoop.executionengine.mapReduceLayer.MapReduceLauncher;
import org.apache.pig.backend.hadoop.executionengine.mapReduceLayer.MapReduceOper;
import org.apache.pig.backend.hadoop.executionengine.mapReduceLayer.plans.MROperPlan;
import org.apache.pig.backend.hadoop.executionengine.physicalLayer.PhysicalOperator;
import org.apache.pig.backend.hadoop.executionengine.physicalLayer.plans.PhysicalPlan;
import org.apache.pig.backend.hadoop.executionengine.physicalLayer.relationalOperators.POPackage;
import org.apache.pig.backend.hadoop.executionengine.physicalLayer.relationalOperators.POStore;
import org.apache.pig.data.DataType;
import org.apache.pig.impl.PigContext;
import org.apache.pig.impl.io.FileLocalizer;
import org.apache.pig.impl.logicalLayer.LogicalPlan;
import org.apache.pig.impl.util.ConfigurationValidator;
import org.apache.pig.test.utils.GenPhyOp;
import org.apache.pig.test.utils.LogicalPlanTester;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

@RunWith(JUnit4.class)
public class TestJobSubmission extends junit.framework.TestCase{
    
    
    static PigContext pc;
    String ldFile;
    String expFile;
    PhysicalPlan php = new PhysicalPlan();
    String stFile;
    String hadoopLdFile;
    String grpName;
    Random r = new Random();
    String curDir;
    String inpDir;
    String golDir;
    static MiniCluster cluster = MiniCluster.buildCluster();
    
    static {

        pc = new PigContext(ExecType.MAPREDUCE, cluster.getProperties());
        try {
            pc.connect();
        } catch (ExecException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        GenPhyOp.setPc(pc);
        
    }
    @Before
    public void setUp() throws Exception{
        curDir = System.getProperty("user.dir");
        inpDir = curDir + File.separatorChar + "test/org/apache/pig/test/data/InputFiles/";
        if ((System.getProperty("os.name").toUpperCase().startsWith("WINDOWS")))
            inpDir="/"+FileLocalizer.parseCygPath(inpDir, FileLocalizer.STYLE_WINDOWS);
        golDir = curDir + File.separatorChar + "test/org/apache/pig/test/data/GoldenFiles/";
        if ((System.getProperty("os.name").toUpperCase().startsWith("WINDOWS")))
            golDir="/"+FileLocalizer.parseCygPath(golDir, FileLocalizer.STYLE_WINDOWS);
    }
    
    @After
    public void tearDown() throws Exception {
    }
    
    @AfterClass
    public static void oneTimeTearDown() throws Exception {
        cluster.shutDown();
    }
    
/*    private void generateInput(int numTuples) throws ExecException{
        
        DataBag inpDb = GenRandomData.genRandSmallTupDataBag(r, numTuples, 1000);
        
        POProject proj = new POProject(new OperatorKey("", r.nextLong()));
        Tuple t = new DefaultTuple();
        t.append(inpDb);
        proj.attachInput(t);
        proj.setColumn(0);
        proj.setOverloaded(true);
        proj.setResultType(DataType.TUPLE);
        
        List<PhysicalOperator> inps = new ArrayList<PhysicalOperator>();
        inps.add(proj);
        
        POStore str = new POStore(new OperatorKey("", r.nextLong()));
        str.setInputs(inps);
        
        FileSpec fSpec = new FileSpec(ldFile, new FuncSpec(PigStorage.class.getName()));
        
        str.setSFile(fSpec);
        str.setPc(pc);
        str.store();
    }
    
    private void setUp1(boolean gen) throws Exception {
        ldFile = "file:" + inpDir + "jsTst1.txt";
        expFile = ldFile;
        stFile = "jsTst1";
        grpName = "jobSubTst1";
        
        if(gen){
            generateInput(100);
            return;
        }
        
        hadoopLdFile = FileLocalizer.hadoopify(ldFile, pc);

        FileSpec LFSpec = new FileSpec(hadoopLdFile,new FuncSpec(PigStorage.class.getName()));
        FileSpec SFSpec = new FileSpec(stFile, new FuncSpec(PigStorage.class.getName()));

        POLoad ld = new POLoad(new OperatorKey("", r.nextLong()), true);
        POStore st = new POStore(new OperatorKey("", r.nextLong()));
        ld.setPc(pc);
        ld.setLFile(LFSpec);
        st.setPc(pc);
        st.setSFile(SFSpec);
        
        php.add(ld);
        php.add(st);
        php.connect(ld, st);
     }

//    @Test
    public void testCompile1() throws Exception {
        boolean gen = false;

        setUp1(gen);
        
        if(gen)
            return;

        submit();
        
        assertEquals(true, FileLocalizer.fileExists(stFile, pc));
        
        FileSpec fSpecExp = new FileSpec(expFile, new FuncSpec(PigStorage.class.getName()));
        FileSpec fSpecAct = new FileSpec(stFile, new FuncSpec(PigStorage.class.getName()));
        
        assertEquals(true, TestHelper.areFilesSame(fSpecExp, fSpecAct, pc));
    }
    
    private void setUp2(boolean gen) throws Exception {
        ldFile = "file:" + inpDir + "jsTst2.txt";
        expFile = ldFile;
        stFile = "jsTst2";
        grpName = "jobSubTst2";
        
        if(gen){
            generateInput(1000);
            return;
        }
        
        hadoopLdFile = FileLocalizer.hadoopify(ldFile, pc);

        FileSpec LFSpec = new FileSpec(hadoopLdFile, new FuncSpec(PigStorage.class.getName()));
        FileSpec SFSpec = new FileSpec(stFile,new FuncSpec(PigStorage.class.getName()));

        POLoad ld = new POLoad(new OperatorKey("", r.nextLong()), true);
        POStore st = new POStore(new OperatorKey("", r.nextLong()));
        ld.setPc(pc);
        ld.setLFile(LFSpec);
        st.setPc(pc);
        st.setSFile(SFSpec);
        
        php.add(ld);
        php.add(st);
        php.connect(ld, st);
     }

//    @Test
    public void testCompile2() throws Exception {
        boolean gen = false;

        setUp2(gen);
        
        if(gen)
            return;

        submit();
        
        assertEquals(true, FileLocalizer.fileExists(stFile, pc));
        
        FileSpec fSpecExp = new FileSpec(expFile,new FuncSpec(PigStorage.class.getName()));
        FileSpec fSpecAct = new FileSpec(stFile,new FuncSpec(PigStorage.class.getName()));
        
        assertEquals(true, TestHelper.areFilesSame(fSpecExp, fSpecAct, pc));
    }
    
    private void setUp3(boolean gen) throws Exception {
        ldFile = "file:" + inpDir + "jsTst1.txt";
        expFile = "file:" + golDir + "jsTst3";
        stFile = "jsTst3";
        grpName = "jobSubTst3";
        
        if(gen){
            generateInput(1000);
            return;
        }
        
        hadoopLdFile = FileLocalizer.hadoopify(ldFile, pc);

        FileSpec LFSpec = new FileSpec(hadoopLdFile, new FuncSpec(PigStorage.class.getName()));
        FileSpec SFSpec = new FileSpec(stFile, new FuncSpec(PigStorage.class.getName()));

        POLoad ld = new POLoad(new OperatorKey("", r.nextLong()), true);
        POStore st = new POStore(new OperatorKey("", r.nextLong()));
        ld.setPc(pc);
        ld.setLFile(LFSpec);
        st.setPc(pc);
        st.setSFile(SFSpec);
        
        int[] flds = {0,1};
        Tuple sample = new DefaultTuple();
        sample.append(new String("S"));
        sample.append(new Integer("10"));
        
        POForEach fe = GenPhyOp.topForEachOPWithPlan(flds , sample);
        
        POFilter fl = GenPhyOp.topFilterOpWithProj(1, 500, GenPhyOp.LT);
        
        php.add(ld);
        php.add(fe);
        php.connect(ld, fe);
        
        php.add(fl);
        php.connect(fe, fl);
        
        php.add(st);
        php.connect(fl, st);
     }

//    @Test
    public void testCompile3() throws Exception {
        boolean gen = false;

        setUp3(gen);
        
        if(gen)
            return;

        submit();
        
        assertEquals(true, FileLocalizer.fileExists(stFile, pc));
        
        FileSpec fSpecExp = new FileSpec(expFile, new FuncSpec(PigStorage.class.getName(), new String[]{","}));
        FileSpec fSpecAct = new FileSpec(stFile,new FuncSpec(PigStorage.class.getName()));
        
        assertEquals(true, TestHelper.areFilesSame(fSpecExp, fSpecAct, pc));
    }
    
    private void setUp4(boolean gen) throws Exception {
        ldFile = "file:" + inpDir + "jsTst1.txt";
        expFile = "file:" + golDir + "jsTst4";
        stFile = "jsTst4";
        grpName = "jobSubTst4";
        
        if(gen){
            generateInput(1000);
            return;
        }
        
        hadoopLdFile = FileLocalizer.hadoopify(ldFile, pc);

        FileSpec LFSpec = new FileSpec(hadoopLdFile,new FuncSpec(PigStorage.class.getName()));
        FileSpec SFSpec = new FileSpec(stFile,new FuncSpec(PigStorage.class.getName()));

        POLoad ld = new POLoad(new OperatorKey("", r.nextLong()), true);
        POStore st = new POStore(new OperatorKey("", r.nextLong()));
        ld.setPc(pc);
        ld.setLFile(LFSpec);
        st.setPc(pc);
        st.setSFile(SFSpec);
        
        POSplit spl = GenPhyOp.topSplitOp();
        POFilter fl1 = GenPhyOp.topFilterOpWithProjWithCast(1, 200, GenPhyOp.LT);
        POFilter fl2 = GenPhyOp.topFilterOpWithProjWithCast(1, 800, GenPhyOp.GT);
        
        POUnion un = GenPhyOp.topUnionOp();
        
        php.add(ld);
        php.add(spl);
        php.connect(ld, spl);
        
        php.add(fl1);
        php.connect(spl, fl1);
        
        php.add(fl2);
        php.connect(spl, fl2);
        
        php.add(un);
        php.connect(fl1, un);
        php.connect(fl2, un);
        
        php.add(st);
        php.connect(un, st);
     }

//    @Test
    public void testCompile4() throws Exception {
        boolean gen = false;

        setUp4(gen);
        
        if(gen)
            return;
        
        submit();
        
        assertEquals(true, FileLocalizer.fileExists(stFile, pc));
        
        FileSpec fSpecExp = new FileSpec(expFile, new FuncSpec(PigStorage.class.getName(), new String[]{","}));
        FileSpec fSpecAct = new FileSpec(stFile,new FuncSpec(PigStorage.class.getName()));
        
        assertEquals(true, TestHelper.areFilesSame(fSpecExp, fSpecAct, pc));
        
    }
    
    private void setUp5(boolean gen) throws Exception {
        ldFile = "file:" + inpDir + "jsTst5.txt";
        expFile = ldFile;
        stFile = "jsTst5";
        grpName = "jobSubTst5";
        
        if(gen){
            generateInput(1000);
            return;
        }
        
        hadoopLdFile = FileLocalizer.hadoopify(ldFile, pc);

        FileSpec LFSpec = new FileSpec(hadoopLdFile, new FuncSpec(PigStorage.class.getName(), new String[]{","}));
        FileSpec SFSpec = new FileSpec(stFile,new FuncSpec(PigStorage.class.getName()));

        POLoad ld = new POLoad(new OperatorKey("", r.nextLong()), true);
        POStore st = new POStore(new OperatorKey("", r.nextLong()));
        ld.setPc(pc);
        ld.setLFile(LFSpec);
        st.setPc(pc);
        st.setSFile(SFSpec);
        
        Tuple sample = new DefaultTuple();
        sample.append("S");
        sample.append(1);
        POLocalRearrange lr = GenPhyOp.topLocalRearrangeOPWithPlan(0, 1, sample);
        
        POGlobalRearrange gr = GenPhyOp.topGlobalRearrangeOp();
        
        POPackage pk = GenPhyOp.topPackageOp();
        pk.setKeyType(DataType.INTEGER);
        pk.setNumInps(1);
        boolean[] inner = {false}; 
        pk.setInner(inner);
        
        POForEach fe = GenPhyOp.topForEachOPWithPlan(1);
        
        php.add(ld);
        php.add(lr);
        php.connect(ld, lr);
        
        php.add(gr);
        php.connect(lr, gr);
        
        php.add(pk);
        php.connect(gr, pk);
        
        php.add(fe);
        php.connect(pk, fe);
        
        php.add(st);
        php.connect(fe, st);
     }

    @Test
    public void testCompile5() throws Exception {
        boolean gen = false;

        setUp5(gen);
        
        if(gen)
            return;
        
        submit();
        
        assertEquals(true, FileLocalizer.fileExists(stFile, pc));
        
        FileSpec fSpecExp = new FileSpec(expFile, new FuncSpec(PigStorage.class.getName(), new String[]{","}));
        FileSpec fSpecAct = new FileSpec(stFile,new FuncSpec(PigStorage.class.getName()));
        
        assertEquals(true, TestHelper.areFilesSame(fSpecExp, fSpecAct, pc));
        
    }*/
    
    @Test
    public void testJobControlCompilerErr() throws Exception {
    	LogicalPlanTester planTester = new LogicalPlanTester() ;
    	planTester.buildPlan("a = load 'input';");
    	LogicalPlan lp = planTester.buildPlan("b = order a by $0;");
    	PhysicalPlan pp = Util.buildPhysicalPlan(lp, pc);
    	POStore store = GenPhyOp.dummyPigStorageOp();
    	pp.addAsLeaf(store);
    	MROperPlan mrPlan = Util.buildMRPlan(pp, pc);
    	
    	for(MapReduceOper mro: mrPlan.getLeaves()) {
    		if(mro.reducePlan != null) {
    			PhysicalOperator po = mro.reducePlan.getRoots().get(0);
    			if(po instanceof POPackage) {
    				((POPackage)po).setKeyType(DataType.BAG);
    				mro.setGlobalSort(true);
    			}
    		}
    	}
    	
        HExecutionEngine exe = pc.getExecutionEngine();
        ConfigurationValidator.validatePigProperties(exe.getConfiguration());
        Configuration conf = ConfigurationUtil.toConfiguration(exe.getConfiguration());
        JobControlCompiler jcc = new JobControlCompiler(pc, conf);
        try {
        	jcc.compile(mrPlan, "Test");
        } catch (JobCreationException jce) {
            assertTrue(jce.getErrorCode() == 1068);
        }
    }
    
    @Test
    public void testDefaultParallel() throws Throwable {
        pc.defaultParallel = 100;
        
        LogicalPlanTester planTester = new LogicalPlanTester() ;
        planTester.buildPlan("a = load 'input';");
        LogicalPlan lp = planTester.buildPlan("b = group a by $0;");
        PhysicalPlan pp = Util.buildPhysicalPlan(lp, pc);
        POStore store = GenPhyOp.dummyPigStorageOp();
        pp.addAsLeaf(store);
        MROperPlan mrPlan = Util.buildMRPlan(pp, pc);

        HExecutionEngine exe = pc.getExecutionEngine();
        ConfigurationValidator.validatePigProperties(exe.getConfiguration());
        Configuration conf = ConfigurationUtil.toConfiguration(exe.getConfiguration());
        JobControlCompiler jcc = new JobControlCompiler(pc, conf);
        
        JobControl jobControl = jcc.compile(mrPlan, "Test");
        Job job = jobControl.getWaitingJobs().get(0);
        int parallel = job.getJobConf().getNumReduceTasks();

        assertTrue(parallel==100);
        
        pc.defaultParallel = -1;        
    }
    
    @Test
    public void testDefaultParallelInSort() throws Throwable {
        pc.defaultParallel = 100;
        
        LogicalPlanTester planTester = new LogicalPlanTester() ;
        planTester.buildPlan("a = load 'input';");
        LogicalPlan lp = planTester.buildPlan("b = order a by $0;");
        PhysicalPlan pp = Util.buildPhysicalPlan(lp, pc);
        POStore store = GenPhyOp.dummyPigStorageOp();
        pp.addAsLeaf(store);
        MROperPlan mrPlan = Util.buildMRPlan(pp, pc);

        HExecutionEngine exe = pc.getExecutionEngine();
        ConfigurationValidator.validatePigProperties(exe.getConfiguration());
        Configuration conf = ConfigurationUtil.toConfiguration(exe.getConfiguration());
        JobControlCompiler jcc = new JobControlCompiler(pc, conf);
        
        // Get the sort job
        Iterator<MapReduceOper> iter = mrPlan.getKeys().values().iterator();
        int counter = 0;
        while (iter.hasNext()) {
            MapReduceOper op = iter.next();
            counter++;
            if (op.isGlobalSort()) {
                assertTrue(op.getRequestedParallelism()==100);
            }
        }
        assertEquals(3, counter);

        pc.defaultParallel = -1;        
    }
    
    @Test
    public void testDefaultParallelInSkewJoin() throws Throwable {
        pc.defaultParallel = 100;
        
        LogicalPlanTester planTester = new LogicalPlanTester() ;
        planTester.buildPlan("a = load 'input';");
        planTester.buildPlan("b = load 'input';");
        LogicalPlan lp = planTester.buildPlan("c = join a by $0, b by $0 using \"skewed\";");
        PhysicalPlan pp = Util.buildPhysicalPlan(lp, pc);
        POStore store = GenPhyOp.dummyPigStorageOp();
        pp.addAsLeaf(store);
        MROperPlan mrPlan = Util.buildMRPlan(pp, pc);

        HExecutionEngine exe = pc.getExecutionEngine();
        ConfigurationValidator.validatePigProperties(exe.getConfiguration());
        Configuration conf = ConfigurationUtil.toConfiguration(exe.getConfiguration());
        JobControlCompiler jcc = new JobControlCompiler(pc, conf);
        
        // Get the skew join job
        Iterator<MapReduceOper> iter = mrPlan.getKeys().values().iterator();
        int counter = 0;
        while (iter.hasNext()) {
            MapReduceOper op = iter.next();
            counter++;
            if (op.isSkewedJoin()) {
                assertTrue(op.getRequestedParallelism()==100);
            }
        }
        assertEquals(3, counter);
        
        pc.defaultParallel = -1;        
    }

    @Test
    public void testReducerNumEstimation() throws Exception{
       // use the estimation
        LogicalPlanTester planTester = new LogicalPlanTester(pc) ;
        Util.copyFromLocalToCluster(cluster, "test/org/apache/pig/test/data/passwd", "/passwd");
        planTester.buildPlan("a = load '/passwd';");
        LogicalPlan lp = planTester.buildPlan("b = group a by $0;");
        PhysicalPlan pp = Util.buildPhysicalPlan(lp, pc);
        POStore store = GenPhyOp.dummyPigStorageOp();
        pp.addAsLeaf(store);
        MROperPlan mrPlan = Util.buildMRPlan(pp, pc);
               
        pc.getConf().setProperty("pig.exec.reducers.bytes.per.reducer", "100");
        pc.getConf().setProperty("pig.exec.reducers.max", "10");
        HExecutionEngine exe = pc.getExecutionEngine();
        ConfigurationValidator.validatePigProperties(exe.getConfiguration());
        Configuration conf = ConfigurationUtil.toConfiguration(exe.getConfiguration());
        JobControlCompiler jcc = new JobControlCompiler(pc, conf);
        JobControl jc=jcc.compile(mrPlan, "Test");
        Job job = jc.getWaitingJobs().get(0);
        long reducer=Math.min((long)Math.ceil(new File("test/org/apache/pig/test/data/passwd").length()/100.0), 10);
        assertEquals(job.getJobConf().getLong("mapred.reduce.tasks",10), reducer);
        
        // use the PARALLEL key word, it will override the estimated reducer number
        planTester = new LogicalPlanTester(pc) ;
        planTester.buildPlan("a = load '/passwd';");
        lp = planTester.buildPlan("b = group a by $0 PARALLEL 2;");
        pp = Util.buildPhysicalPlan(lp, pc);
        store = GenPhyOp.dummyPigStorageOp();
        pp.addAsLeaf(store);
        mrPlan = Util.buildMRPlan(pp, pc);
               
        pc.getConf().setProperty("pig.exec.reducers.bytes.per.reducer", "100");
        pc.getConf().setProperty("pig.exec.reducers.max", "10");
        exe = pc.getExecutionEngine();
        ConfigurationValidator.validatePigProperties(exe.getConfiguration());
        conf = ConfigurationUtil.toConfiguration(exe.getConfiguration());
        jcc = new JobControlCompiler(pc, conf);
        jc=jcc.compile(mrPlan, "Test");
        job = jc.getWaitingJobs().get(0);
        assertEquals(job.getJobConf().getLong("mapred.reduce.tasks",10), 2);
        
        // the estimation won't take effect when it apply to non-dfs or the files doesn't exist, such as hbase
        planTester = new LogicalPlanTester(pc) ;
        planTester.buildPlan("a = load 'hbase://passwd' using org.apache.pig.backend.hadoop.hbase.HBaseStorage('c:f1 c:f2');");
        lp = planTester.buildPlan("b = group a by $0 ;");
        pp = Util.buildPhysicalPlan(lp, pc);
        store = GenPhyOp.dummyPigStorageOp();
        pp.addAsLeaf(store);
        mrPlan = Util.buildMRPlan(pp, pc);
                
        pc.getConf().setProperty("pig.exec.reducers.bytes.per.reducer", "100");
        pc.getConf().setProperty("pig.exec.reducers.max", "10");
        exe = pc.getExecutionEngine();
        ConfigurationValidator.validatePigProperties(exe.getConfiguration());
        conf = ConfigurationUtil.toConfiguration(exe.getConfiguration());
        jcc = new JobControlCompiler(pc, conf);
        jc=jcc.compile(mrPlan, "Test");
        job = jc.getWaitingJobs().get(0);
        assertEquals(job.getJobConf().getLong("mapred.reduce.tasks",10), 1);
    }
    
    @Test
    public void testReducerNumEstimationForOrderBy() throws Exception{
       // use the estimation
        pc.getProperties().setProperty("pig.exec.reducers.bytes.per.reducer", "100");
        pc.getProperties().setProperty("pig.exec.reducers.max", "10");
        
        LogicalPlanTester planTester = new LogicalPlanTester(pc) ;
        Util.copyFromLocalToCluster(cluster, "test/org/apache/pig/test/data/passwd", "/passwd");
        planTester.buildPlan("a = load '/passwd';");
        LogicalPlan lp = planTester.buildPlan("b = order a by $0;");
        PhysicalPlan pp = Util.buildPhysicalPlan(lp, pc);
        POStore store = GenPhyOp.dummyPigStorageOp();
        pp.addAsLeaf(store);

        MROperPlan mrPlan = Util.buildMRPlanWithOptimizer(pp, pc);
        assertEquals(2, mrPlan.size());     
        
        MapReduceOper sort = mrPlan.getLeaves().get(0);        
        long reducer=Math.min((long)Math.ceil(new File("test/org/apache/pig/test/data/passwd").length()/100.0), 10);
        assertEquals(reducer, sort.getRequestedParallelism());
        
        // use the PARALLEL key word, it will override the estimated reducer number
        planTester = new LogicalPlanTester(pc) ;
        planTester.buildPlan("a = load '/passwd';");
        lp = planTester.buildPlan("b = order a by $0 PARALLEL 2;");
        pp = Util.buildPhysicalPlan(lp, pc);
        store = GenPhyOp.dummyPigStorageOp();
        pp.addAsLeaf(store);
        
        mrPlan = Util.buildMRPlanWithOptimizer(pp, pc);               
        assertEquals(2, mrPlan.size());     
        
        sort = mrPlan.getLeaves().get(0);        
        assertEquals(2, sort.getRequestedParallelism());
        
        // the estimation won't take effect when it apply to non-dfs or the files doesn't exist, such as hbase
        planTester = new LogicalPlanTester(pc) ;
        planTester.buildPlan("a = load 'hbase://passwd' using org.apache.pig.backend.hadoop.hbase.HBaseStorage('c:f1 c:f2');");
        lp = planTester.buildPlan("b = order a by $0 ;");
        pp = Util.buildPhysicalPlan(lp, pc);
        store = GenPhyOp.dummyPigStorageOp();
        pp.addAsLeaf(store);
 
        mrPlan = Util.buildMRPlanWithOptimizer(pp, pc);               
        assertEquals(2, mrPlan.size());     
        
        sort = mrPlan.getLeaves().get(0);
        
        assertEquals(1, sort.getRequestedParallelism());
        
        // test order by with three jobs (after optimization)
        planTester = new LogicalPlanTester(pc) ;
        planTester.buildPlan("a = load '/passwd';");
        planTester.buildPlan("b = foreach a generate $0, $1, $2;");
        lp = planTester.buildPlan("c = order b by $0;");
        pp = Util.buildPhysicalPlan(lp, pc);
        store = GenPhyOp.dummyPigStorageOp();
        pp.addAsLeaf(store);
        
        mrPlan = Util.buildMRPlanWithOptimizer(pp, pc);
        assertEquals(3, mrPlan.size());     
        
        sort = mrPlan.getLeaves().get(0);       
        assertEquals(reducer, sort.getRequestedParallelism());
    }
    
    private void submit() throws Exception{
        assertEquals(true, FileLocalizer.fileExists(hadoopLdFile, pc));
        MapReduceLauncher mrl = new MapReduceLauncher();
        mrl.launchPig(php, grpName, pc);  
    }
}
