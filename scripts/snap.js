const { SerialPort } = require('serialport');
const Firmata = require('./lib/firmata');
const args = require('minimist')(process.argv.slice(2))
let uart_port = args['port'] || "COM4";
if (uart_port)
{
    const during = (args['during'] || 5) * 1000;
    const port = new SerialPort({ path: uart_port, baudRate: 115200, autoOpen: false });
    port.open(function (err)
    {
        if (err)
        {
            return console.log('Error opening port: ', err.message)
        }
    })
}