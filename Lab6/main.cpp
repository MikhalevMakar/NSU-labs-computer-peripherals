#include <libusb.h>
#include <iomanip>
#include <cassert>
#include <iostream>


using namespace std;

void printParametrs(libusb_device** device, int countDeviceUSB);
void printDevice(libusb_device *dev);

int main() {
    libusb_device** device; //структура, представляющая USB-устройство, обнаруженно
    libusb_context* libContext = NULL; //позволяет вашей программе использовать  2 библиотеки

    assert(libusb_init(&libContext) == 0); //инициализация работы с libusb

    ssize_t countDeviceUSB;
    countDeviceUSB = libusb_get_device_list(libContext, &device);

    if(countDeviceUSB < 0) {
        cerr << "Error: USB device list not received.\n";
        return 1;
    }

    cout << "Devices found: " << countDeviceUSB << "\n";

    printParametrs(device, countDeviceUSB);

    libusb_free_device_list(device, 1);
    libusb_exit(libContext);
    return 0;
}

void printParametrs(libusb_device** device, int countDeviceUSB) {
    std::cout << "DeviceClass";
    std::cout << std::setw(15) << "ManufacturerId";
    std::cout << std::setw(15) << "ProductId";
    std::cout << std::setw(15) << "SerialNumber\n";

    for(ssize_t i = 0; i < countDeviceUSB; ++i) {
        printDevice(device[i]);
    }
}

void printDevice(libusb_device* device) {
    libusb_device_descriptor descriptor;
    assert(libusb_get_device_descriptor(device, &descriptor) == 0);
    cout << setw(5) << hex << descriptor.bDeviceClass;
    cout << setw(5) << hex << descriptor.idVendor;
    cout << setw(5) << hex << descriptor.idProduct;

    libusb_device_handle* handle;
    assert(libusb_open(device, &handle) == 0);
    unsigned char lineForSerialNumber[255];
    int readValue = 0;

    if (handle != NULL && descriptor.iSerialNumber != 0) {
        readValue = libusb_get_string_descriptor_ascii(handle,
                                                       descriptor.iSerialNumber,
                                                       lineForSerialNumber,
                                                       sizeof(lineForSerialNumber));
    }

    if(readValue == 0) std::cout<< setw(10) << "NULL\n";
    else {
        std::cout << setw(10) << lineForSerialNumber << "\n";
    }

    libusb_close(handle);
}
