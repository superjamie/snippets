/*
 * sixread.c - Reads PS3 controller master and device Bluetooth MACs
 * AUTHOR: Jamie Bainbridge - jamie.bainbridge@gmail.com
 * LICENSE: GPL v2 https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 * DESCRIPTION: Quick and dirty hack of pabr's and android bluez' sixpair.c
 * http://www.pabr.org/sixlinux/sixlinux.en.html
 * https://android.googlesource.com/platform/external/bluetooth/bluez/+/gingerbread/input/sixpair.c
 * COMPILE: gcc -o sixread sixread.c -lusb
 * RUN: sudo ./sixread
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <usb.h>

#define VENDOR 0x054c
#define PRODUCT 0x0268

#define USB_DIR_IN 0x80
#define USB_DIR_OUT 0

void fatal(char *msg)
{
	perror(msg);
	exit(1);
}

void show_master(usb_dev_handle *devh, int itfnum)
{
	printf("Current Bluetooth master: ");
	unsigned char msg[8];
	int res = usb_control_msg
	    (devh, /* device */
	    USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE, /* request */
	    0x01, /* request type */
	    0x03f5, /* value */
	    itfnum, /* index  */
	    (void*)msg, /* data */
	    sizeof(msg), /* size */
	    5000 /* timeout */
	    );

	if (res < 0) {
		perror("Error getting master MAC");
		return;
	}
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
	    msg[2], msg[3], msg[4], msg[5], msg[6], msg[7]);
}

void show_bdaddr(usb_dev_handle *devh, int itfnum)
{
	printf("Device Bluetooth address: ");
	unsigned char msg[17];
	int res = usb_control_msg(
	    devh, /* device */
	    USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE, /* req */
	    0x01, /* reqtype */
	    0x03f2, /* value */
	    itfnum, /* index  */
	    (void*)msg, /* data */
	    sizeof(msg), /* size */
	    5000 /* timeout */
	    );

	if (res < 0) {
		perror("Error getting device MAC");
		return;
	}
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
	    msg[4], msg[5], msg[6], msg[7], msg[8], msg[9]);
}

void process_device(int argc, char **argv, struct usb_device *dev,
                    struct usb_config_descriptor *cfg, int itfnum) {
  int mac[6], have_mac=0;

  usb_dev_handle *devh = usb_open(dev);
  if ( ! devh ) fatal("usb_open");

  usb_detach_kernel_driver_np(devh, itfnum);

  int res = usb_claim_interface(devh, itfnum);
  if ( res < 0 ) fatal("usb_claim_interface");

  show_master(devh, itfnum);
  show_bdaddr(devh, itfnum);

  usb_close(devh);
}

int main(int argc, char *argv[]) {  

  usb_init();
  if ( usb_find_busses() < 0 ) fatal("usb_find_busses");
  if ( usb_find_devices() < 0 ) fatal("usb_find_devices");
  struct usb_bus *busses = usb_get_busses();
  if ( ! busses ) fatal("usb_get_busses");

  int found = 0;

  struct usb_bus *bus;
  for ( bus=busses; bus; bus=bus->next ) {
    struct usb_device *dev;
    for ( dev=bus->devices; dev; dev=dev->next) {
      struct usb_config_descriptor *cfg;
      for ( cfg = dev->config;
            cfg < dev->config + dev->descriptor.bNumConfigurations;
            ++cfg ) {
        int itfnum;
        for ( itfnum=0; itfnum<cfg->bNumInterfaces; ++itfnum ) {
          struct usb_interface *itf = &cfg->interface[itfnum];
          struct usb_interface_descriptor *alt;
          for ( alt = itf->altsetting;
                alt < itf->altsetting + itf->num_altsetting;
                ++alt ) {
            if ( dev->descriptor.idVendor == VENDOR &&
                 dev->descriptor.idProduct == PRODUCT &&
                 alt->bInterfaceClass == 3 ) {
              process_device(argc, argv, dev, cfg, itfnum);
              ++found;
            }
          }
        }
      }
    }
  }

  if ( ! found ) printf("No controller found on USB busses.\n");
  return 0;

}
