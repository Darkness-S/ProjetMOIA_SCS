/*
********************************************************************
 *
 *  Programme : interfaceCJavaProlog.java
 *
 *  ecrit par : Sammy Loudiyi.
 *
 *  resume :    Serveur du jeu "Colonnes de trois".
 *
 *  date :      11 / 05 / 17
 *
 *******************************************************************
 */

/* Import pour socket java */
import java.net.ServerSocket;
import java.net.Socket;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;

public class interfaceCJavaProlog {  
	static InputStream input;
	static boolean c;
	static String coul;
	static Socket s;

	public static void recevoir() throws IOException{
		System.out.println("recevoir");
		byte[] commC = new byte[256];
	    is.read(commC);
	    String test = new String(commC);
	    System.out.println(test);
	}
	public static void recevoir3() throws IOException{
		System.out.println("fin prog");
		byte[] commC = new byte[256];
	    is.read(commC);
	    String test = new String(commC);
	    System.out.println(test);
	}