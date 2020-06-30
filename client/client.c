#include "gdbusCommon.h"
#include "client.h"

static GMainLoop *pLoop = NULL;
static GDBusConnection *pConnection = NULL;
static ComGdbusDemo *pProxy = NULL;

static void setNameMethod(const gchar *in_arg, gchar** out_arg, GError** pError);
static gboolean sendSignalHandler(ComGdbusDemo *object, const gint *arg, gpointer userdata);
static void *run(void *arg);

static void setNameMethod(const gchar *in_arg, gchar** out_arg, GError** pError){
	
	com_gdbus_demo_call_set_name_sync (pProxy, in_arg, out_arg, NULL, pError);
	
	if (*pError == NULL) {
        g_print("Client setNameMethod is called, in_arg = %s out_arg = %s.\n", in_arg, *out_arg);
        g_free(*out_arg);
    } else {
        g_print("Failed to call setNameMethod. Reason: %s.\n", (*pError)->message);
        g_error_free(*pError);
    }
}

static gboolean sendSignalHandler(ComGdbusDemo *object, const gint *arg, gpointer userdata){
	 
	g_print("sendSignalHandler: Signal value: %d.\n", arg);

    return TRUE;
}

static void *run(void* arg)
{
    g_main_loop_run(pLoop);
}

bool initDBusCommunication(void){
	
	bool bRet = TRUE;
    GError *pConnError = NULL;
    GError *pProxyError = NULL;
	
	do{
		bRet = TRUE;
		pLoop = g_main_loop_new(NULL,FALSE); 
		
		pConnection = g_bus_get_sync(COM_GDBUS_DEMO_BUS, NULL, &pConnError);
		if(pConnError == NULL){
			pProxy = com_gdbus_demo_proxy_new_sync(pConnection,
						 G_DBUS_PROXY_FLAGS_NONE,
						 COM_GDBUS_DEMO_NAME,
						 COM_GDBUS_DEMO_OBJECT_PATH,
						 NULL,
						 &pProxyError);
			if(pProxy == 0){
				g_print("initDBusCommunication: Create proxy failed. Reason: %s.\n", pConnError->message);
				g_error_free(pProxyError);
				bRet = FALSE;
			}else{
				g_print("initDBusCommunication: Create proxy successfully. \n");
			}
		}else{
			g_print("initDBusCommunication: Failed to connect to dbus. Reason: %s.\n", pConnError->message);
            g_error_free(pConnError);
            bRet = FALSE;
		}
	}while(bRet == FALSE);
					 
	if(bRet == TRUE){
		g_signal_connect(pProxy, "send-signal", G_CALLBACK(sendSignalHandler), NULL);
	}else{
		g_print("initDBusCommunication: Failed to connect signal.  \n");
	}

	return bRet;
}

int main(void){

	pthread_t tid;
	
	const gchar *intputArg = "TODD";
    gchar *outputArg = NULL;
    GError *gMethoderror = NULL;

    initDBusCommunication();

    setNameMethod(intputArg, &outputArg, &gMethoderror);
    
	pthread_create(&tid,NULL,run,NULL);

    while (1)
	{
		/* code */
	}
	
    return 0;
}