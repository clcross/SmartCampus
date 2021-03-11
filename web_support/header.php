<?php
        $servername = "10.127.98.154";
        $dbUsername = "cam";
        $dbPassword = 'XSW@2wsx';
        $dbName     = "SmartCampus";

        $conn = new mysqli($servername, $dbUsername, $dbPassword, $dbName);

        if(!$conn){
                echo "Connection Error";
                die("Connection Failed: ".$conn->connect_error);
        }

?>

<style>
<?php include "main.css";?>
</style>

<html>

<head>
    <meta charset="utf-8">
    <title>Smart Campus</title>
</head>

<body>
	<ul>
		<li><a href="index.php">Back to Map</a></li>
		<li><h1>Smart Campus</h1></li>
	</ul>
</body>

</html>

