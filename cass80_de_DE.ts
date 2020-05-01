<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de_DE">
<context>
    <name>AboutDlg</name>
    <message>
        <location filename="dialogs/aboutdlg.ui" line="14"/>
        <source>About …</source>
        <translation>Über …</translation>
    </message>
    <message>
        <location filename="dialogs/aboutdlg.ui" line="23"/>
        <source>Cass80 is a tool to load, analyze, disassemble or list, modify, and save EACA Colour-Genie 2000 and Video-Genie 3003 (which was a TRS-80 clone) cassette images.</source>
        <translation>Cass80 ist ein Werkzeug zum laden, analyisieren, disassemblieren oder listen, bearbeiten und laden von EACA Colour-Genie 2000 und Video-Genie 3003 (ein TRS-80 Klon) Kassettendateien.</translation>
    </message>
    <message>
        <location filename="dialogs/aboutdlg.ui" line="96"/>
        <source>This is a Work-In-Progress and I use it to analyze and clean up old &quot;*.cas&quot; images I had lying around in my archives for many years. The results will be added to &lt;a href=&quot;https://mamedev.org/&quot;&gt;MAME&lt;/a&gt; hash list (softlist) for the Colour Genie cassettes.</source>
        <translation>Es ist WIP (work in progress) und ich nutze es, um alte &quot;*.cas&quot; Dateien zu analysieren und aufzuräumen, die ich in meinen Archiven für viele Jahre herumliegen hatte. Die Ergebnisse werden zu den &lt;a href=&quot;https://mamedev.org/&quot;&gt;MAME&lt;/a&gt; Hash-Tabellen (Softlist) für Colour-Genie Kassetten hinzugefügt werden.</translation>
    </message>
    <message>
        <location filename="dialogs/aboutdlg.cpp" line="9"/>
        <source>About %1</source>
        <translation>Über %1</translation>
    </message>
</context>
<context>
    <name>Cass80Handler</name>
    <message>
        <location filename="src/cass80handler.cpp" line="319"/>
        <source>Header Name: %1</source>
        <translation>Info Name: %1</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="325"/>
        <source>Header Author: %1</source>
        <translation>Info Autor: %1</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="331"/>
        <source>Header Copyright: %1</source>
        <translation>Info Copyright: %1</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="350"/>
        <source>Header Description: %1</source>
        <translation>Info Beschreibung: %1</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="409"/>
        <source>HEADER: %1 %2 %3 %4 %5 %6 %7 %8</source>
        <translation>HEADER: %1 %2 %3 %4 %5 %6 %7 %8</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="425"/>
        <source>SYSTEM tape: &apos;%1&apos;</source>
        <translation>SYSTEM Band: &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="437"/>
        <source>TRS-80 BASIC tape: &apos;%1&apos;</source>
        <translation>TRS-80 BASIC Band: &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="448"/>
        <source>Colour Genie BASIC tape: &apos;%1&apos;</source>
        <translation>Colour-Genie BASIC Band: &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="499"/>
        <source>SYSTEM data block: %1 (0x%2) at %3h</source>
        <translation>SYSTEM Datenblock: %1 (0x%2) an Addresse %3h</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="556"/>
        <source>SYSTEM entry point: %1 (0x%2)</source>
        <translation>SYSTEM Einsprungadresse: %1 (0x%2)</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="579"/>
        <source>BASIC end: %1 (0x%2)</source>
        <translation>BASIC Ende: %1 (0x%2)</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="610"/>
        <source>BASIC line: #%1 at %2h</source>
        <translation>BASIC Zeile: Nr. %1 bei %2h</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="661"/>
        <source>Loaded %1 blocks (%2 bytes).</source>
        <translation>%1 Blöcke geladen (%2 Bytes).</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="662"/>
        <source>SHA1 of data: %1.</source>
        <translation>SHA1 der Daten: %1.</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="695"/>
        <source>Found MOVER at address %1h</source>
        <translation>MOVER an Adresse %1h gefunden</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="701"/>
        <source>MOVER src:0x%1 dst:0x%2 size:0x%3 entry:0x%4</source>
        <translation>MOVER src:0x%1 dst:0x%2 size:0x%3 entry:0x%4</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="822"/>
        <source>Invalid machine - none of EG2000 or TRS80 were detected.</source>
        <translation>Ungültige Maschine - weder EG2000 noch TRS80 wurden erkannt.</translation>
    </message>
    <message>
        <location filename="src/cass80handler.cpp" line="851"/>
        <source>Block length is wrong: %1 (2 &lt;= blen &lt;= 256</source>
        <translation>Blocklänge ist falsch: %1 (2 &lt;= blen &lt;= 256)</translation>
    </message>
</context>
<context>
    <name>Cass80Main</name>
    <message>
        <location filename="cass80main.ui" line="14"/>
        <source>Cassette Manager for TRS80 and EG2000</source>
        <translation>Kassetten-Manager für TRS80 und EG2000</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="31"/>
        <source>Listing</source>
        <translation>Listing</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="71"/>
        <source>Log output</source>
        <translation>Logausgabe</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="122"/>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="132"/>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="143"/>
        <source>&amp;Help</source>
        <translation>&amp;Hilfe</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="154"/>
        <source>toolBar</source>
        <translation></translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="169"/>
        <source>&amp;Load …</source>
        <translation>&amp;Laden …</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="172"/>
        <source>Load TRS80 / Colour Genie cassette file</source>
        <translation>Lade TRS80 / Colour-Genie Kassettendatei</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="175"/>
        <source>Ctrl+L</source>
        <translation>Strg+L</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="184"/>
        <source>&amp;Quit</source>
        <translation>&amp;Quit</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="187"/>
        <source>Ctrl+Q</source>
        <translation>Strg+Q</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="196"/>
        <source>&amp;Save …</source>
        <translation>&amp;Speichern …</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="199"/>
        <source>Ctrl+S</source>
        <translation>Strg+S</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="208"/>
        <source>Undo &amp;lmoffset</source>
        <translation>&amp;lmoffset rückgängig machen</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="211"/>
        <source>Ctrl+U</source>
        <translation>Strg+U</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="220"/>
        <source>&amp;Information …</source>
        <translation>&amp;Informationen …</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="229"/>
        <source>&amp;Preferences</source>
        <translation>&amp;Einstellungen</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="234"/>
        <source>&amp;About …</source>
        <translation>&amp;Über …</translation>
    </message>
    <message>
        <location filename="cass80main.ui" line="239"/>
        <source>About &amp;Qt</source>
        <translation>Über &amp;Qt</translation>
    </message>
    <message>
        <location filename="src/cass80main.cpp" line="81"/>
        <source>Cassette Manager for TRS80 and EG2000 - Version %1</source>
        <translation>Kassetten-Manager für TRS80 und EG2000 - Version %1</translation>
    </message>
    <message>
        <location filename="src/cass80main.cpp" line="156"/>
        <source>Cassette (*.cas)</source>
        <translation>Kassette (*.cas)</translation>
    </message>
    <message>
        <location filename="src/cass80main.cpp" line="180"/>
        <source>Found LMOFFSET loader</source>
        <translation>LMOFFSET Loader gefunden</translation>
    </message>
</context>
<context>
    <name>cass80InfoDlg</name>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="20"/>
        <source>Cassette image information</source>
        <translation>Kasstendatei Informationen</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="50"/>
        <source>Contents SHA1</source>
        <translation>Inhalts SHA1</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="65"/>
        <source>Filename</source>
        <translation>Dateiname</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="99"/>
        <source>Machine</source>
        <translation>Maschine</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="114"/>
        <source>Block length</source>
        <translation>Blocklänge</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="148"/>
        <source>Format</source>
        <translation>Format</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="179"/>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="213"/>
        <source>Number of blocks</source>
        <translation>Anzahl der Blöcke</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="303"/>
        <source>Copyright</source>
        <translation>Copyright</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="359"/>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="392"/>
        <source>Description</source>
        <translation>Beschreibung</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.ui" line="407"/>
        <source>File CRC32</source>
        <translation>Datei CRC32</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.cpp" line="65"/>
        <source>EG2000</source>
        <translation>EG2000</translation>
    </message>
    <message>
        <location filename="dialogs/casinfodlg.cpp" line="68"/>
        <source>TRS80</source>
        <translation>TRS80</translation>
    </message>
</context>
<context>
    <name>preferencesDlg</name>
    <message>
        <location filename="dialogs/preferencesdlg.ui" line="14"/>
        <source>Dialog</source>
        <translation>Dialog</translation>
    </message>
    <message>
        <location filename="dialogs/preferencesdlg.ui" line="30"/>
        <source>Use integrated TTF for Colour Genie</source>
        <translation>Nutze eingebauten Colour-Genie TTF</translation>
    </message>
    <message>
        <location filename="dialogs/preferencesdlg.ui" line="37"/>
        <source>Prefer upper case in listings</source>
        <translation>Bevorzuge Großbuchstaben in Listings</translation>
    </message>
</context>
</TS>
