lib =  libui_gtk_dialog_error.o libui_gtk_gadget_move.o libui_gtk_gadget_window.o libui_gtk_button.o libui_gtk_image.o libui_gtk_label.o libui_gtk_entry.o

%.o: lib/%.c
	gcc -c $< -o $@ -g `pkg-config --cflags gtk+-2.0` -I/usr/include -lcurl

objects = main.o ephy-spinner.o vodafone_gadget_send.o vodafone_gadget_func.o vodafone_gadget_cairo.o vodafone_gadget_errors.o  vodafone_gadget_xml.o vodafone_gadget_setting.o vodafone_gadget_about.o vodafone_gadget_notify.o

%.o: src/%.c
	gcc -c $< -o $@ -g `pkg-config --cflags gtk+-2.0` `xml2-config --cflags` -lgthread-2.0 -lcurl -lnotify -I/usr/include

compile: $(lib) $(objects)
	gcc $(lib) $(objects) -o vodafone-gadget `pkg-config --libs gtk+-2.0` `xml2-config --libs` -lgthread-2.0 -lcurl -lnotify -I/usr/include

clean:
	rm *.o -fv
