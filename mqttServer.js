let mosca = require('mosca');
let mqtt = require('mqtt')

let settings = { port:1883 }
let server = new mosca.Server(settings);
let client  = mqtt.connect('mqtt://172.17.60.203');

server.on('ready', function(){
    client.subscribe('sensedData');
});

client.on('message', function (topic, message) {
    console.log(message.toString());
    if(topic == "sensedData"){
        let json = JSON.parse(message.toString());
    }
});