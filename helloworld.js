var builder = require('botbuilder');

var connector = new builder.ConsoleConnector().listen();
var bot = new builder.UniversalBot(connector);

bot.dialog('/', [
   function (session) {
        builder.Prompts.text(session, "Hello... What's your latitude?");
    },
    function (session, results) {
        session.userData.lat = results.response;
        builder.Prompts.text(session, "Okay, now to what's your longitude?"); 
    },
    function (session, results) {
        session.userData.lon = results.response;
        builder.Prompts.text(session, "How much do you want to spend?"); 
    },
    function (session, results) {
        session.userData.cost = results.response;
        builder.Prompts.text(session, "How far are you willing to drive in total?"); 
    },
    function (session, results) {
        session.userData.distance = results.response;


        var execFile = require('child_process').execFile;

        execFile('./src/SilioNavTest',['./src/locationdata.txt', session.userData.lat, session.userData.lon, session.userData.cost, session.userData.distance], function(err, data) {
            if(err) {
                console.log(err)
            }else
                session.send(data.toString());                       
        }); 
        session.send("Perfect. Let me plan your night:");
    }
]);

