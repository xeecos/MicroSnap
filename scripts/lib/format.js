class Format
{
    constructor()
    {

    }
    float2bytes(val)
    {
        let buf = new ArrayBuffer(4);
        let view = new DataView(buf);
        view.setFloat32(0,val);
        return [view.getUint8(3),view.getUint8(2),view.getUint8(1),view.getUint8(0)]
    }
    short2bytes(val)
    {
        let buf = new ArrayBuffer(2);
        let view = new DataView(buf);
        view.setUint16(0,val);
        return [view.getUint8(1),view.getUint8(0)]
    }
    long2bytes(val)
    {
        let buf = new ArrayBuffer(4);
        let view = new DataView(buf);
        view.setUint32(0,val);
        return [view.getUint8(3),view.getUint8(2),view.getUint8(1),view.getUint8(0)]
    }
}
module.exports = new Format();