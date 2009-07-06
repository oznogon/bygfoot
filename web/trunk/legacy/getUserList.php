<?php
include('../config.php');
if (isset($_POST["u_password"]) && ($_POST["u_password"]=="<fill_IN>") && (isset($_POST["u_name"])) && ($_POST["u_name"]=="<FILL_IN>")) {
$phpbb_root_path = '../';
// Connect to the database
$link = mysql_connect($dbhost, $dbuser, $dbpasswd);
mysql_select_db($dbname);
 
require 'exportcsv.inc.php';
 
exportMysqlToCsv();
} else {
  print "<form method=\"post\"><p align=\"center\">Please enter your password for access<br>";
  print "<input name=\"u_name\" type=\"text\" size=\"25\" maxlength=\"10\">";
  print "<input name=\"u_password\" type=\"password\" size=\"25\" maxlength=\"10\"><input value=\"Login\" type=\"submit\"></p></form>";
}

?>
