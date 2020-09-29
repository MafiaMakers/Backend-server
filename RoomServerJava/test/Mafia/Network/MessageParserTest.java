package Mafia.Network;

import org.junit.Assert;
import org.junit.Test;

import Mafia.Randomizer;
import Mafia.Network.MessageParser;
import Mafia.Network.MessageJ;

public class MessageParserTest {

    @Test
    public void WrapParseRandomTest(){
        ClientJ testClient = new ClientJ(Randomizer.randint(), Randomizer.randint());

        MessageJ testMessage = new MessageJ(Randomizer.randint(), Randomizer.randint(),
                Randomizer.randint(), Randomizer.randint(), testClient,
                "Qwer\0\n\t][@#!#$%^&*()_+}{tyu".toCharArray());

        byte[] buffer = MessageParser.wrap_message(testMessage);

        MessageJ resultMessage = MessageParser.parse_message(buffer, buffer.length, testClient.Getip(), testClient.Getport());

        Assert.assertTrue("Parsed and original messages differ!" , resultMessage.equals(testMessage));
    }

    @Test
    public void WrapParseEqualsTest(){
        ClientJ testClient = new ClientJ(14, 15);

        MessageJ testMessage = new MessageJ(10, 11, 12, 13, testClient,
                "Qwer\0\n\t][@#!#$%^&*()_+}{tyu".toCharArray());

        byte[] buffer = MessageParser.wrap_message(testMessage);

        MessageJ resultMessage = MessageParser.parse_message(buffer, buffer.length, testClient.Getip(), testClient.Getport());

        Assert.assertTrue("Parsed and original messages differ!" , resultMessage.equals(testMessage));
    }
}
