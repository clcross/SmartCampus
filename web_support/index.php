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
    <title>Smart Campus Cafeteria</title>
</head>


<body>
<?php
header('Refresh: 2; URL=/index.php');

echo "<h1>Smart Campus</h1>";

    $sql = "SELECT Location, Population FROM Campus";
    $result = $conn->query($sql);
    
    echo "<table>
	<tr>
	    <th>Location</th>
	    <th>Population</th>
	</tr>";
    
    if ($result->num_rows > 0){
	while($row = $result->fetch_assoc()){
		echo "
		    <tr>
			<td>" . $row["Location"] . " </td> 
			<td>" . $row["Population"] . " </td>
		    </tr>";
	}
    }else{
	echo "0 results";
    }
$conn->close();
echo "</table>";
?>

</body>



</html>
