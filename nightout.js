var builder = require('botbuilder');

/*
For this guide we’ll just chat with the bot from a console window so we’ll use the ConsoleConnector class. 
In the future when you’re ready to deploy your bot to real channels you’ll want to swap out the ConsoleConnector 
for a ChatConnector configured with your bots App ID & Password from the Bot Framework portal.
*/

var connecter = new builder.ConsoleConnector().listen();
var bot = new builder.UniversalBot(connecter);

bot.dialog('/', [
        function (session) {
            builder.Prompts.text(session, "Hey, what can I do for you?");
        }
    ]
);
    

