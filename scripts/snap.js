const { SerialPort } = require('serialport');
const Firmata = require('./lib/firmata');
const args = require('minimist')(process.argv.slice(2))
let uart_port = args['port'] || "COM6";
if (uart_port)
{
    // let test = [0xf0, 0x03, 0x00 , 0x3d , 0x00 , 0x18 , 0x00 , 0x00 , 0x40 , 0x0f , 0x60 , 0xf7];
    // test.map(c=>{
    //     Firmata.parse(c)
    // })

    const port = new SerialPort({ path: uart_port, baudRate: 115200, autoOpen: false });
    port.open(async function (err)
    {
        if (err)
        {
            return console.log('Error opening port: ', err.message)
        }
        console.log("opened",Buffer.from(Firmata.forward(1000, 1)));
        let arr = Firmata.forward(2356, 14.5);
        for(let i=0;i<arr.length;i++)
        {
            port.write(Buffer.from([arr[i]]));
            await delay(1);
        }
        // port.write(Buffer.from(Firmata.backward(1000, 1)));
    });
    port.on("data",(msg)=>{
        console.log(msg.toString());
    });
}
function delay(time)
{
    return new Promise(resolve=>{
        setTimeout(resolve,time);
    })
}