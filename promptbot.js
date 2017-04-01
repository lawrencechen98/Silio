var builder = require('botbuilder');

var connecter = new builder.ConsoleConnector().listen();
var bot = new builder.UniversalBot(connecter);

bot.dialog('/', [
    function(session) {
        builder.Prompts.text(session, "Hello, whats your name asshole?");
    }, 
    function(session, results) {
        
    }
])