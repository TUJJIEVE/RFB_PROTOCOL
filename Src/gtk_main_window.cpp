#include <gtk/gtk.h>
#include <iostream>
#include <cstdio>

const int MAX_PTRS_IN_VOIDPOINTERS = 10;
const int MAX_IP_LENGTH = 15 ;
const int MAX_PWD_LENGTH = 20;
struct IP_Password_pair {
    char ip[MAX_IP_LENGTH] ;
    char pwd[MAX_PWD_LENGTH] ;
};
struct voidPointers
{
    int nPtrs = 0 ;
    void * ptrs[MAX_PTRS_IN_VOIDPOINTERS];
};.


void destroy(GtkWidget *widget,gpointer   data );
void on_btn_connect_clicked(GtkWidget *widget,gpointer   data);



int main(int argc, char *argv[])
{
    GtkBuilder * builder ;
    GtkWidget * window , * connectButton  , * inputIPaddr , * inputPassword   , * inputIPaddrMsg , * inputPasswordMsg  ;
    
    
    voidPointers voidPtrs ;

    gtk_init(&argc ,&argv) ;

    builder = gtk_builder_new();

    if(builder == nullptr){
        std::cerr << "builder didnot formed correctly" << std::endl ;
    }
    
    int fileStatus = gtk_builder_add_from_file (builder, "./../GUI/test1.glade", NULL);
    if(fileStatus == 0){
        std::cerr << "file didnot opened correctly" << std::endl ;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    if(window == nullptr){
        std::cerr << "window widget didnot formed correctly" << std::endl ;
    }
    connectButton = GTK_WIDGET(gtk_builder_get_object(builder, "connect_button"));
    if(connectButton == nullptr){
        std::cerr << "connect_button widget didnot formed correctly" << std::endl ;
    }
    inputIPaddr = GTK_WIDGET(gtk_builder_get_object(builder, "ipAddr"));
    if(inputIPaddr == NULL){
        std::cerr << "inputIPaddr widget didnot formed correctly" << std::endl ;
    }
    inputPassword = GTK_WIDGET(gtk_builder_get_object(builder, "password"));
    if(inputPassword == NULL){
        std::cerr << "inputPassword widget didnot formed correctly" << std::endl ;
    }
      
    // inputIPaddrMsg = GTK_WIDGET(gtk_builder_get_object(builder, "ipAddrMsg"));
    // if(inputIPaddrMsg == NULL){
    //     std::cerr << "inputIPaddr Msg widget didnot formed correctly" << std::endl ;
    // }
    // inputPasswordMsg = GTK_WIDGET(gtk_builder_get_object(builder, "passwordMsg"));
    // if(inputPasswordMsg == NULL){
    //     std::cerr << "inputPassword Msg widget didnot formed correctly" << std::endl ;
    // }
    voidPtrs.nPtrs = 4;
    voidPtrs.ptrs[0] = (void*)inputIPaddr;
    voidPtrs.ptrs[1] = (void*)inputPassword;
    voidPtrs.ptrs[2] = (void*)inputIPaddrMsg;
    voidPtrs.ptrs[3] = (void*)inputPasswordMsg;
    //program closes on clicking x symbol//
    g_signal_connect (window, "destroy", G_CALLBACK (destroy),NULL);
    g_signal_connect (connectButton,"clicked",G_CALLBACK (on_btn_connect_clicked),(void *)&voidPtrs);
    gtk_builder_connect_signals(builder, NULL);


    g_object_unref(builder);
    gtk_widget_show(window);                
    gtk_main();

    return 0;
}

void on_btn_connect_clicked(GtkWidget *widget,gpointer   data){
    
    voidPointers * pointers = (voidPointers *) data ;
    GtkWidget * ipAddr , * password  ,* ipAddrMsg , * passwordMsg;
    ipAddr = (GtkWidget *)pointers->ptrs[0];
    password = (GtkWidget *)pointers->ptrs[1];
    IP_Password_pair ipp;
    //MAX_IP_LENGTH = 15 
    //MAX_PWD_LENGTH = 20 
    // sprintf(ipp.ip ,"%15s" , gtk_entry_get_text((GtkEntry *)ipAddr));
    // // memcpy((void *)ipp.ip,gtk_entry_get_text((GtkEntry *)password),15*sizeof(char));
    // gtk_text_buffer_set_text((GtkTextBuffer * )ipAddrMsg,"enter ip address here",21);
    // gtk_text_buffer_set_text((GtkTextBuffer * )passwordMsg,"enter pwd address here",22);
    strcpy(ipp.ip , gtk_entry_get_text((GtkEntry *)ipAddr));//,MAX_IP_LENGTH
    strcpy(ipp.pwd , gtk_entry_get_text((GtkEntry *)password));//,MAX_PWD_LENGTH
    printf("user clicked button\nipaddr : %s\npasswd : %s\n",ipp.ip,ipp.pwd);
    // gtk_entry_set_text( (GtkEntry *)ipAddr,"");
    // gtk_entry_set_text( (GtkEntry *)password,"");
    bool isPasswordCorrect = false ;

    
    if(strcmp((const char *)ipp.ip,"127.0.0.1") == 1){
        printf("used same ip address \n");
    }


    // isPasswordCorrect = checkPassword and assign bool


    if(isPasswordCorrect){
        gtk.window.destroy() ;
        //-----------------------
        //run function which should run after password is correct

        //------------------------
    }else{
        printf("incorrect password\n");
    }
    


}


void destroy( GtkWidget *widget,gpointer   data )
{
   gtk_main_quit ();
}