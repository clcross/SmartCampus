<style>
<?php include "main.css";?>
<?php include "header.php";?>
</style>

<html>

<head>
    <meta charset="utf-8">
    <title>Smart Campus Student Center</title>
</head>


<body>
<?php
header('Refresh: 2; URL=/studentcenter.php');

echo "<h1>Student Center</h1>";

    $sql = "SELECT Location, Population FROM Campus WHERE Location = 'Student Center'";
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

