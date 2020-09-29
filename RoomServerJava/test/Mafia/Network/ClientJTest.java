package Mafia.Network;

import Mafia.Randomizer;
import org.junit.Assert;
import org.junit.Test;

import java.util.Random;

public class ClientJTest {

    @Test
    public void SetipTest(){
        int testIP = 127001;

        ClientJ testClient = new ClientJ();
        testClient.Setip(testIP);
        Assert.assertEquals("IP differs!", testClient.Getip(), testIP);
    }

    @Test
    public void SetportTest(){
        int testPort = 48945;

        ClientJ testClient = new ClientJ();
        testClient.Setport(testPort);
        Assert.assertEquals("Port differs!", testClient.Getport(), testPort);
    }

    @Test
    public void ConstructorTest(){
        int testIP = 179;
        int testPort = 189;
        ClientJ testClient = new ClientJ(testIP, testPort);

        Assert.assertEquals(testClient.Getip(), testIP);
        Assert.assertEquals(testClient.Getport(), testPort);
    }

    @Test
    public void RandomConstructorTest(){
        int testIP = Randomizer.randint();
        int testPort = Randomizer.randint();
        ClientJ testClient = new ClientJ(testIP, testPort);

        Assert.assertEquals(testClient.Getip(), testIP);
        Assert.assertEquals(testClient.Getport(), testPort);
    }
}
