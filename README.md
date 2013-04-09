Arduino-IrRemote
================
本项目源自：http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html

参照 Zend 命名规范重写，主要是驼峰命名，并解决原 IRremote 库和 Arduino 自带函数 tone() 冲突的问题。

名字空间：IrRemote

* 发射器
> IrRemote::Transmitter

* 接收器
> IrRemote::Receiver

example:

    IrRemote::Transmitter irTransmitter;
    IrRemote::Receiver irReciver;
    IrRemote::Data irData;
    
    if (irReciver.decode(&irData)) {
      irTransmitter.send(&irData);
    }

