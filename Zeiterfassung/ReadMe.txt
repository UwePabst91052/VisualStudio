================================================================================
    MICROSOFT FOUNDATION CLASS-BIBLIOTHEK: Zeiterfassung-Projekt�bersicht
===============================================================================

Der Anwendungs-Assistent hat diese Zeiterfassung-Anwendung f�r Sie 
erstellt. Diese Anwendung zeigt nicht nur die Grundlagen der Verwendung von
Microsoft Foundation Classes, sondern dient auch als Ausgangspunkt f�r das
Schreiben Ihrer Anwendung.

Diese Datei enth�lt eine Zusammenfassung dessen, was Sie in jeder der Dateien
finden, aus denen Ihre Zeiterfassung�Anwendung besteht.

Zeiterfassung.vcproj
    Dies ist die Hauptprojektdatei f�r VC++-Projekte, die mithilfe eines 
    Anwendungs-Assistenten erstellt werden. 
    Sie enth�lt Informationen �ber die Version von Visual C++, in der die Datei 
    erzeugt wurde, sowie Informationen �ber die Plattformen, Konfigurationen und 
    Projektfunktionen, die mit dem Anwendungs-Assistenten ausgew�hlt wurden.

Zeiterfassung.h
    Dies ist die Hauptheaderdatei f�r die Anwendung. Sie enth�lt weitere
    projektspezifische Header (einschlie�lich "Resource.h") und deklariert
    die CZeiterfassungApp-Anwendungsklasse.

Zeiterfassung.cpp
    Dies ist die Hauptquelldatei der Anwendung, die die CZeiterfassungApp-
    Anwendungsklasse enth�lt.

Zeiterfassung.rc
    Dies ist eine Auflistung aller Microsoft Windows-Ressourcen, die das
    Programm verwendet. Sie enth�lt die Symbole, Bitmaps und Cursor, die im 
    Unterverzeichnis "RES" gespeichert werden. Diese Datei kann direkt in 
    Microsoft Visual C++ bearbeitet werden. Ihre Projektressourcen befinden sich 
    in 1031.

res\Zeiterfassung.ico
    Dies ist eine Symboldatei, die als Symbol der Anwendung verwendet wird. 
    Dieses Symbol ist in der Hauptressourcendatei "Zeiterfassung.rc" 
    enthalten.

res\Zeiterfassung.rc2
    Diese Datei enth�lt Ressourcen, die nicht von Microsoft Visual C++
    bearbeitet werden. Sie sollten alle Ressourcen, die nicht mit dem
    Ressourcen-Editor bearbeitet werden k�nnen, in dieser Datei platzieren.

Zeiterfassung.reg
    Dies ist eine REG-Beispieldatei, die Ihnen die Art der Registrierungs-
    einstellungen zeigt, die das Framework f�r Sie festlegt. Sie k�nnen diese 
    als REG-Datei verwenden.
    Datei f�r Ihre Anwendung. Oder l�schen Sie sie einfach, und vertrauen
    Sie auf die standardm��ige RegisterShellFileTypes�Registrierung.


/////////////////////////////////////////////////////////////////////////////

F�r das Hauptrahmenfenster:
    Das Projekt enth�lt eine standardm��ige MFC-Schnittstelle.

MainFrm.h, MainFrm.cpp
    Diese Datei enth�lt die CMainFrame�Rahmenklasse, die von
    CFrameWnd abgeleitet wird und alle SDI-Rahmenfunktionen steuert.

res\Toolbar.bmp
    Diese Bitmapdatei wird verwendet, um gekachelte Bilder f�r die Symbolleiste 
    zu erstellen.
    Die urspr�ngliche Symbolleiste und Statusleiste werden in der CMainFrame-
    Klasse erstellt. Bearbeiten Sie diese Symbolleistenbitmap mithilfe des
    Ressourcen-Editors, und aktualisieren Sie das IDR_MAINFRAME TOOLBAR-Array
    in "Zeiterfassung.rc", um Symbolleisten-Schaltfl�chen hinzuzuf�gen.
/////////////////////////////////////////////////////////////////////////////

Der Anwendungs-Assistent erstellt einen Dokumenttyp und eine Ansicht:

ZeiterfassungDoc.h, ZeiterfassungDoc.cpp � das Dokument
    Diese Dateien enthalten Ihre CZeiterfassungDoc�Klasse. Bearbeiten Sie 
    diese Dateien, um spezielle Dokumentdaten hinzuzuf�gen und Dateispeicherung
    und �ladung (�ber CZeiterfassungDoc::Serialize) zu implementieren.
    Das Dokument weist folgende Zeichenfolgen auf:
        Dateierweiterung:          tkn
        Dateityp-ID:               Zeiterfassung.Document
        Hauptrahmen�berschrift:    T�tigkeitsnachweis
        Dokumenttypname:           Zeiterfassung
        Filtername:                Zeiterfassung Files (*.tkn)
        Neuer Dateikurzname:       Zeiterfassung
        Langer Name des Dateityps: Zeiterfassung.Document

ZeiterfassungView.h, ZeiterfassungView.cpp � die Ansicht des Dokuments
    Diese Dateien enthalten Ihre CZeiterfassungView�Klasse.
    CZeiterfassungView-Objekte werden verwendet, um CZeiterfassungDoc-
    Objekte anzuzeigen.





/////////////////////////////////////////////////////////////////////////////

Weitere Funktionen:

ActiveX-Steuerelemente
    Die Anwendung unterst�tzt die Verwendung von ActiveX-Steuerelementen.

Splitterfenster
    Der Anwendungs-Assistent hat Unterst�tzung f�r Splitterfenster f�r Ihre 
    Anwendungsdokumente hinzugef�gt.

/////////////////////////////////////////////////////////////////////////////

Weitere Standarddateien:

"StdAfx.h", "StdAfx.cpp"
    Diese Dateien werden verwendet, um eine vorkompilierte Headerdatei
    (PCH-Datei) mit dem Namen "Zeiterfassung.pch.pch2 und eine 
    vorkompilierte Typendatei mit dem Namen "StdAfx.obj" zu erstellen.

"Resource.h"
    Dies ist die Standardheaderdatei, die neue Ressourcen-IDs definiert.
    Microsoft Visual C++ liest und aktualisiert diese Datei.

Zeiterfassung.manifest
	Anwendungsmanifestdateien werden von Windows XP verwendet, um eine 
	Anwendungsabh�ngigkeit von verschiedenen Versionen paralleler Assemblys zu 
	beschreiben.
	Das Ladeprogramm verwendet diese Informationen, um die entsprechende 
	Assembly aus dem Assemblycache oder privat aus der Anwendung zu laden. Das
	Anwendungsmanifest kann zur Verteilung als externe Manifestdatei
	enthalten sein, die im gleichen Ordner installiert ist wie die ausf�hrbare 
	Datei der Anwendung, oder sie kann in Form einer Ressource in der 
	ausf�hrbaren Datei enthalten sein. 
/////////////////////////////////////////////////////////////////////////////

Weitere Hinweise:

Der Anwendungs-Assistent verwendet "TODO:", um auf Teile des Quellcodes
hinzuweisen, die Sie erg�nzen oder anpassen sollten.

Wenn Ihre Anwendung MFC in einer freigegebenen DLL verwendet, m�ssen Sie die 
MFC-DLLs verteilen. Wenn die Anwendung eine andere Sprache als die des Gebietsschemas 
des Betriebssystems verwendet, m�ssen Sie au�erdem die entsprechenden lokalisierten Ressourcen "MFC90XXX.DLL" verteilen. Weitere Informationen zu diesen beiden Themen 
finden Sie im Abschnitt zum Verteilen von Visual C++-Anwendungen in der MSDN-
Dokumentation.

/////////////////////////////////////////////////////////////////////////////