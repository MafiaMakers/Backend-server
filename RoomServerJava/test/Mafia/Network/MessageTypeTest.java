package Mafia.Network;

import Mafia.Exceptions.MafiaException;
import Mafia.Network.General.MessageType;
import org.junit.Assert;
import org.junit.Test;

public class MessageTypeTest {
    @Test
    public void MainTest(){
        MessageType mt = MessageType.Text;
        Assert.assertEquals(2, mt.getType());
        mt = MessageType.TestText;
        Assert.assertEquals(1, mt.getType());
    }

    @Test
    public void FromIntTest(){
        int t = 1;
        try {
            MessageType mt = MessageType.from_int(t);
            Assert.assertEquals(t, mt.getType());
        } catch (Exception ex){
            Assert.fail("Exception is thrown!");
        }
    }

    @Test
    public void FromIntExceptionTest(){
        int t = -179;
        try {
            MessageType mt = MessageType.from_int(t);
            Assert.fail("Exception is not thrown!");
        } catch (MafiaException ex){
            
        }
    }
}
