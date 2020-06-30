#include "server.h"
#include "gdbusCommon.h"

static GMainLoop *pLoop = NULL;
static ComGdbusDemo *pSkeleton = NULL;

static gboolean sendSignal(gconstpointer p);
static gboolean setName(ComGdbusDemo *object, GDBusMethodInvocation *invocation, const gchar *in_arg, gpointer user_data);
static void *run(void* arg);

static void GBusAcquired_Callback(GDBusConnection *connection,
                         const gchar *name,
                         gpointer user_data);
static void GBusNameAcquired_Callback (GDBusConnection *connection,
                             const gchar *name,
                             gpointer user_data);
static void GBusNameLost_Callback (GDBusConnection *connection,
                         const gchar *name,
                         gpointer user_data);

static gboolean sendSignal(gconstpointer p)          
{                                                         
    g_print("Server sendSignal is called.\n");            

    gint sig = 6;
    com_gdbus_demo_emit_send_signal (pSkeleton,sig);

    return TRUE;                                          
} 

static gboolean setName(ComGdbusDemo *object,
                           GDBusMethodInvocation *invocation,
                           const gchar           *in_arg,
                           gpointer               user_data)
{
    g_print("Server setName is call. Name is : %s.\n", in_arg);

    in_arg = "Set Name Success";

    com_gdbus_demo_complete_set_name(object, invocation, in_arg);

    return TRUE;
}

static void *run(void* arg)
{
    g_main_loop_run(pLoop);
}

static void GBusAcquired_Callback(GDBusConnection *connection,
                         const gchar *name,
                         gpointer user_data){
    
    GError *pError = NULL;

    pSkeleton = com_gdbus_demo_skeleton_new();

    (void) g_signal_connect(pSkeleton, "handle-set-name", G_CALLBACK(setName), NULL);

    (void) g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(pSkeleton),
                                                connection,
                                                COM_GDBUS_DEMO_OBJECT_PATH,
                                                &pError);

    if(pError == 0){
        g_print("skeleton export successfully. \n");
    }else{
        g_print("Error: Failed to export object. Reason: %s.\n", pError->message);
        g_error_free(pError);
        g_main_loop_quit(pLoop);
        return;
    }
}

static void GBusNameAcquired_Callback (GDBusConnection *connection,
                             const gchar *name,
                             gpointer user_data){
    g_print("GBusNameAcquired_Callback, Acquired bus name: %s \n", COM_GDBUS_DEMO_NAME);
}

static void GBusNameLost_Callback (GDBusConnection *connection,
                         const gchar *name,
                         gpointer user_data){
    if (connection == NULL)
    {
        g_print("GBusNameLost_Callback, Error: Failed to connect to dbus. \n");
    }else{
        g_print("GBusNameLost_Callback, Error: Failed to get dbus name : %s\n", COM_GDBUS_DEMO_NAME);
}
    g_main_loop_quit(pLoop);
}

bool initDBusCommunicationForServer(void){
    
    bool bRet = TRUE;

    pLoop = g_main_loop_new(NULL,FALSE); 

    guint own_id = 
        g_bus_own_name (COM_GDBUS_DEMO_BUS,
                    COM_GDBUS_DEMO_NAME,
                    G_BUS_NAME_OWNER_FLAGS_NONE,
                    &GBusAcquired_Callback,
                    &GBusNameAcquired_Callback,
                    &GBusNameLost_Callback,
                    NULL,
                    NULL);

    g_timeout_add(2000, (GSourceFunc)sendSignal, NULL);

    return bRet;
}


int main(){

    pthread_t tid;
    
    initDBusCommunicationForServer();

    pthread_create(&tid, NULL, run, NULL);

    while(1){
	    /* code */
    }
    return 0;
}

