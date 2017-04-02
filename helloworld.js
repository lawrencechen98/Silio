var builder = require('botbuilder');
var locationDialog = require('botbuilder-location');
var config = require('./config');

/*
 var connector = new builder.ChatConnector({
     appId: config.BOT_ID,
     appPassword: config.BOT_PASSWORD
 });
 */
var connector = new builder.ConsoleConnector().listen();
var bot = new builder.UniversalBot(connector);

bot.dialog('/', [
   function (session) {
        //builder.Prompts.text(session, "Hello... Where are you?");
        var options = {
            prompt: "Where do you want to leave from?",
            useNativeControl: true
        };
        locationDialog.getLocation(session, options);
    },
    function (session, results) {
        session.userData.start = results.response;
        builder.Prompts.text(session, "Okay, now to where?"); 
    },
    function (session, results) {
        session.userData.end = results.response;


        var execFile = require('child_process').execFile;

        execFile('./MyNav',['./src/locationdata.txt', session.userData.start, session.userData.end], function(err, data) {
            if(err) {
                console.log(err)
            }else
                session.send(data);                       
        }); 
        session.send("Perfect. Let me plan your night:");
    }/
]);

