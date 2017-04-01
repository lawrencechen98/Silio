var builder = require('botbuilder');

var connector = new builder.ConsoleConnector().listen();
var bot = new builder.UniversalBot(connector);

bot.dialog('/', [
   function (session) {
        builder.Prompts.text(session, "Hello... Where are you?");
    },
    function (session, results) {
        session.userData.start = results.response;
        builder.Prompts.text(session, "Okay, now to where?"); 
    },
    function (session, results) {
        session.userData.end = results.response;


        var execFile = require('child_process').execFile;

        execFile('./MyNav',['/Users/Lawrence/Desktop/CS32/project4/project4/mapdata.txt', session.userData.start, session.userData.end], function(err, data) {
            if(err) {
                console.log(err)
            }else
                session.send(data.toString());                       
        }); 
        session.send("Perfect. Let me plan your night:");
    }
]);

