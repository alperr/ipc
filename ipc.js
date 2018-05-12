var net = require('net');
var fs = require("fs");

var WRITER_SOCKET_PATH = "/tmp/nodejs_c_bridge.sock";
var READER_SOCKET_PATH = "/tmp/c_nodejs_bridge.sock";
var isCProcessStarted = false;
var readerConnection;
var writerSocket;

function onconnection(readerSocket) // only one connection is accepted
{
	if (isCProcessStarted)
		return;
	
	createWriter();
	readerSocket.on("data",function(d)
	{
		console.log(d);
	});
}

function createWriter()
{
	isCProcessStarted = true;
	writerSocket = net.createConnection(WRITER_SOCKET_PATH);
	writerSocket.on("connect", function()
	{
		console.log("nodejs writer connected to c process");
	});
}

exports.init = function()
{
	readerConnection = net.createServer(function(client) {});
	fs.unlinkSync(READER_SOCKET_PATH);
	readerConnection.listen(READER_SOCKET_PATH);
	readerConnection.on("connection", onconnection);
	console.log("nodejs reader listening");
}

exports.onmessage = function(d){}
exports.send = function(buffer)
{
	// 	var buf = new Buffer(4);
	// 	buf.writeUInt8(0x3, 0);
	// 	buf.writeUInt8(0x4, 1);
	// 	buf.writeUInt8(0x23, 2);
	// 	buf.writeUInt8(0x42, 3);
	// 	writerSocket.write(buf);
	
	if (isCProcessStarted)
	{
		console.log("cant send message, c process has not started yet");
		return;
	}
	
	writerSocket.write(buffer)
}