package Mafia.Network;

import Mafia.Randomizer;
import org.junit.Assert;
import org.junit.Test;

public class MessageJTest {

    @Test
    public void ConstructorTest(){
        int id = 179;
        int type = 11;
        int partsCount = 123;
        int partIndex = 23;
        char[] data = "qwertyuiodfg!@#$%^&*()_+{}[]~::\"\".,?/hjkl".toCharArray();

        ClientJ testClient = new ClientJ(234, 345);

        MessageJ messageJ = new MessageJ(id, partsCount, partIndex, type, testClient, data);

        Assert.assertTrue(String.valueOf(messageJ.Getdata()).equals(String.valueOf(data)));
        Assert.assertTrue(messageJ.Getclient().equals(testClient));
        Assert.assertEquals(messageJ.Gettype(), type);
        Assert.assertEquals(messageJ.Getid(), id);
        Assert.assertEquals(messageJ.GetpartsCount(), partsCount);
        Assert.assertEquals(messageJ.GetpartIndex(), partIndex);
    }

    @Test
    public void RandomConstructorTest(){
        int id = Randomizer.randint();
        int type = Randomizer.randint();
        int partsCount = Randomizer.randint();
        int partIndex = Randomizer.randint();
        char[] data = "qwertyuiodfg!@#$%^&*()_+{}[]~::\"\".,?/hjkl".toCharArray();

        ClientJ testClient = new ClientJ(Randomizer.randint(), Randomizer.randint());

        MessageJ messageJ = new MessageJ(id, partsCount, partIndex, type, testClient, data);

        Assert.assertTrue(String.valueOf(messageJ.Getdata()).equals(String.valueOf(data)));
        Assert.assertTrue(messageJ.Getclient().equals(testClient));
        Assert.assertEquals(messageJ.Gettype(), type);
        Assert.assertEquals(messageJ.Getid(), id);
        Assert.assertEquals(messageJ.GetpartsCount(), partsCount);
        Assert.assertEquals(messageJ.GetpartIndex(), partIndex);
    }

    @Test
    public void ClientTest(){
        ClientJ testClient = new ClientJ(234, 345);
        MessageJ messageJ = new MessageJ();
        messageJ.Setclient(testClient);

        Assert.assertTrue(messageJ.Getclient().equals(testClient));
    }

    @Test
    public void ClientRandomTest(){
        ClientJ testClient = new ClientJ(Randomizer.randint(), Randomizer.randint());
        MessageJ messageJ = new MessageJ();
        messageJ.Setclient(testClient);

        Assert.assertTrue(messageJ.Getclient().equals(testClient));
    }

    @Test
    public void TypeTest(){
        int type = 1;
        MessageJ messageJ = new MessageJ();
        messageJ.Settype(type);

        Assert.assertTrue(messageJ.Gettype() ==type);
    }

    @Test
    public void TypeRandomTest(){
        int type = Randomizer.randint();
        MessageJ messageJ = new MessageJ();
        messageJ.Settype(type);

        Assert.assertEquals(messageJ.Gettype(), type);
    }

    @Test
    public void IdTest(){
        int id = 1;
        MessageJ messageJ = new MessageJ();
        messageJ.Setid(id);

        Assert.assertEquals(messageJ.Getid(), id);
    }

    @Test
    public void IdRandomTest(){
        int id = Randomizer.randint();
        MessageJ messageJ = new MessageJ();
        messageJ.Setid(id);

        Assert.assertEquals(messageJ.Getid(), id);
    }

    @Test
    public void PartsCountTest(){
        int partsCount = 22;
        MessageJ messageJ = new MessageJ();
        messageJ.SetpartsCount(partsCount);

        Assert.assertEquals(messageJ.GetpartsCount(), partsCount);
    }

    @Test
    public void PartsCountRandomTest(){
        int partsCount = Randomizer.randint();
        MessageJ messageJ = new MessageJ();
        messageJ.SetpartsCount(partsCount);

        Assert.assertEquals(messageJ.GetpartsCount(), partsCount);
    }

    @Test
    public void PartIndexTest(){
        int partIndex = 11;
        MessageJ messageJ = new MessageJ();
        messageJ.SetpartIndex(partIndex);

        Assert.assertEquals(messageJ.GetpartIndex(), partIndex);
    }

    @Test
    public void PartIndexRandomTest(){
        int partIndex = Randomizer.randint();
        MessageJ messageJ = new MessageJ();
        messageJ.SetpartIndex(partIndex);

        Assert.assertEquals(messageJ.GetpartIndex(), partIndex);
    }

    @Test
    public void DataTest(){
        char[] data = "qwertyuiopsdfghjk!@#$%^&*()1234567890{}[];':\",./<>?~`".toCharArray();
        MessageJ messageJ = new MessageJ();
        messageJ.Setdata(data);

        Assert.assertTrue(String.valueOf(messageJ.Getdata()).equals(String.valueOf(data)));
    }
}
