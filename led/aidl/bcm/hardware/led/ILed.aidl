package bcm.hardware.led;

interface ILed {
    void setValue(in byte value);
    byte getValue();
}