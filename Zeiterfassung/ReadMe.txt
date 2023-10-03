================================================================================
    MICROSOFT FOUNDATION CLASS-BIBLIOTHEK: Zeiterfassung-Projektübersicht
===============================================================================

Der Anwendungs-Assistent hat diese Zeiterfassung-Anwendung für Sie 
erstellt. Diese Anwendung zeigt nicht nur die Grundlagen der Verwendung von
Microsoft Foundation Classes, sondern dient auch als Ausgangspunkt für das
Schreiben Ihrer Anwendung.

Diese Datei enthält eine Zusammenfassung dessen, was Sie in jeder der Dateien
finden, aus denen Ihre Zeiterfassung–Anwendung besteht.

Zeiterfassung.vcproj
    Dies ist die Hauptprojektdatei für VC++-Projekte, die mithilfe eines 
    Anwendungs-Assistenten erstellt werden. 
    Sie enthält Informationen über die Version von Visual C++, in der die Datei 
    erzeugt wurde, sowie Informationen über die Plattformen, Konfigurationen und 
    Projektfunktionen, die mit dem Anwendungs-Assistenten ausgewählt wurden.

Zeiterfassung.h
    Dies ist die Hauptheaderdatei für die Anwendung. Sie enthält weitere
    projektspezifische Header (einschließlich "Resource.h") und deklariert
    die CZeiterfassungApp-Anwendungsklasse.

Zeiterfassung.cpp
    Dies ist die Hauptquelldatei der Anwendung, die die CZeiterfassungApp-
    Anwendungsklasse enthält.

Zeiterfassung.rc
    Dies ist eine Auflistung aller Microsoft Windows-Ressourcen, die das
    Programm verwendet. Sie enthält die Symbole, Bitmaps und Cursor, die im 
    Unterverzeichnis "RES" gespeichert werden. Diese Datei kann direkt in 
    Microsoft Visual C++ bearbeitet werden. Ihre Projektressourcen befinden sich 
    in 1031.

res\Zeiterfassung.ico
    Dies ist eine Symboldatei, die als Symbol der Anwendung verwendet wird. 
    Dieses Symbol ist in der Hauptressourcendatei "Zeiterfassung.rc" 
    enthalten.

res\Zeiterfassung.rc2
    Diese Datei enthält Ressourcen, die nicht von Microsoft Visual C++
    bearbeitet werden. Sie sollten alle Ressourcen, die nicht mit dem
    Ressourcen-Editor bearbeitet werden können, in dieser Datei platzieren.

Zeiterfassung.reg
    Dies ist eine REG-Beispieldatei, die Ihnen die Art der Registrierungs-
    einstellungen zeigt, die das Framework für Sie festlegt. Sie können diese 
    als REG-Datei verwenden.
    Datei für Ihre Anwendung. Oder löschen Sie sie einfach, und vertrauen
    Sie auf die standardmäßige RegisterShellFileTypes–Registrierung.


/////////////////////////////////////////////////////////////////////////////

Für das Hauptrahmenfenster:
    Das Projekt enthält eine standardmäßige MFC-Schnittstelle.

MainFrm.h, MainFrm.cpp
    Diese Datei enthält die CMainFrame–Rahmenklasse, die von
    CFrameWnd abgeleitet wird und alle SDI-Rahmenfunktionen steuert.

res\Toolbar.bmp
    Diese Bitmapdatei wird verwendet, um gekachelte Bilder für die Symbolleiste 
    zu erstellen.
    Die ursprüngliche Symbolleiste und Statusleiste werden in der CMainFrame-
    Klasse erstellt. Bearbeiten Sie diese Symbolleistenbitmap mithilfe des
    Ressourcen-Editors, und aktualisieren Sie das IDR_MAINFRAME TOOLBAR-Array
    in "Zeiterfassung.rc", um Symbolleisten-Schaltflächen hinzuzufügen.
/////////////////////////////////////////////////////////////////////////////

Der Anwendungs-Assistent erstellt einen Dokumenttyp und eine Ansicht:

ZeiterfassungDoc.h, ZeiterfassungDoc.cpp – das Dokument
    Diese Dateien enthalten Ihre CZeiterfassungDoc–Klasse. Bearbeiten Sie 
    diese Dateien, um spezielle Dokumentdaten hinzuzufügen und Dateispeicherung
    und –ladung (über CZeiterfassungDoc::Serialize) zu implementieren.
    Das Dokument weist folgende Zeichenfolgen auf:
        Dateierweiterung:          tkn
        Dateityp-ID:               Zeiterfassung.Document
        Hauptrahmenüberschrift:    Tätigkeitsnachweis
        Dokumenttypname:           Zeiterfassung
        Filtername:                Zeiterfassung Files (*.tkn)
        Neuer Dateikurzname:       Zeiterfassung
        Langer Name des Dateityps: Zeiterfassung.Document

ZeiterfassungView.h, ZeiterfassungView.cpp – die Ansicht des Dokuments
    Diese Dateien enthalten Ihre CZeiterfassungView–Klasse.
    CZeiterfassungView-Objekte werden verwendet, um CZeiterfassungDoc-
    Objekte anzuzeigen.





/////////////////////////////////////////////////////////////////////////////

Weitere Funktionen:

ActiveX-Steuerelemente
    Die Anwendung unterstützt die Verwendung von ActiveX-Steuerelementen.

Splitterfenster
    Der Anwendungs-Assistent hat Unterstützung für Splitterfenster für Ihre 
    Anwendungsdokumente hinzugefügt.

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
	Anwendungsabhängigkeit von verschiedenen Versionen paralleler Assemblys zu 
	beschreiben.
	Das Ladeprogramm verwendet diese Informationen, um die entsprechende 
	Assembly aus dem Assemblycache oder privat aus der Anwendung zu laden. Das
	Anwendungsmanifest kann zur Verteilung als externe Manifestdatei
	enthalten sein, die im gleichen Ordner installiert ist wie die ausführbare 
	Datei der Anwendung, oder sie kann in Form einer Ressource in der 
	ausführbaren Datei enthalten sein. 
/////////////////////////////////////////////////////////////////////////////

Weitere Hinweise:

Der Anwendungs-Assistent verwendet "TODO:", um auf Teile des Quellcodes
hinzuweisen, die Sie ergänzen oder anpassen sollten.

Wenn Ihre Anwendung MFC in einer freigegebenen DLL verwendet, müssen Sie die 
MFC-DLLs verteilen. Wenn die Anwendung eine andere Sprache als die des Gebietsschemas 
des Betriebssystems verwendet, müssen Sie außerdem die entsprechenden lokalisierten Ressourcen "MFC90XXX.DLL" verteilen. Weitere Informationen zu diesen beiden Themen 
finden Sie im Abschnitt zum Verteilen von Visual C++-Anwendungen in der MSDN-
Dokumentation.

/////////////////////////////////////////////////////////////////////////////