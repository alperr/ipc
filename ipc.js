var NET = require('net');
var FS = require("fs");

var WRITER_SOCKET_PATH = "/tmp/ntoc.sock";
var READER_SOCKET_PATH = "/tmp/cton.sock";
var is_connected = false;
var reader;
var writer_sock;
var msg_handler;

function onconnection(reader_sock) // only one connection is accepted
{
	console.log("c process detected");
	if (is_connected)
		return;
	
	create_writer();
	reader_sock.on("data",function(d)
	{
		console.log(d);
	});
}

function create_writer()
{
	writer_sock = NET.createConnection(WRITER_SOCKET_PATH);
	writer_sock.on("connect", function()
	{
		is_connected = true;
		console.log("nodejs writer connected to c process");
	});
}

function start(onmsg)
{
	msg_handler = onmsg;
	reader = NET.createServer(function(client) {});
	try{FS.unlinkSync(READER_SOCKET_PATH);}catch(e){}	
	reader.listen(READER_SOCKET_PATH);
	reader.on("connection", onconnection);
	console.log("nodejs reader listening");
}

function send(buffer)
{
	// 	var buf = new Buffer(4);
	// 	buf.writeUInt8(0x3, 0);
	// 	buf.writeUInt8(0x4, 1);
	// 	buf.writeUInt8(0x23, 2);
	// 	buf.writeUInt8(0x42, 3);
	// 	writerSocket.write(buf);
	
	if (!is_connected)
	{
		console.log("cant send message, c process has not started yet");
		return;
	}	
	writer_sock.write(buffer);
}

exports.start = start;
exports.send = send;