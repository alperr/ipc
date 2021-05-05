var IPC = require("./ipc.js");
IPC.start("./path-of-binary.exe");
IPC.onmessage = function()
{
	IPC.send("test message");
}