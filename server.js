var builder = require('botbuilder');
var restify = require('restify');
var locationDialog = require('botbuilder-location');

var connector = new builder.ChatConnector({
     appId: "f9eef841-e41f-4022-ab22-2ad7ea840755",
     appPassword: "BXwLXXAoxYjniWVeHgL9LE0"
});

var bot = new builder.UniversalBot(connector);

bot.dialog('/', [
    function (session) {
       
        //builder.Prompts.text(session, "Hello... Where are you?");
        // var options = {
        //     prompt: "Where do you want to leave from?",
        //     useNativeControl: true
        // };
        //locationDialog.getLocation(session, options);
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

        execFile(__dirname + '/src/SilioNavTest',[__dirname + '/src/locationdata.txt', session.userData.lat, session.userData.lon, session.userData.cost, session.userData.distance], function(err, data) {
            session.send("We in here!");
            if(err) {
                session.send(err.toString());
                console.log(err)
            }else {
                session.send("success");
                session.send(data.toString());
            }                       
        }); 
        session.send("Perfect. Let me plan your night:");
    }
]);

var server = restify.createServer();

server.post('/api/messages', connector.listen());

server.listen(process.env.port || 3978, function () {
        console.log('%s listening to %s', server.name, server.url); 
});
