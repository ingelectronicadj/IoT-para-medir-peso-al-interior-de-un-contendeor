<html>
<body>
<?php
include 'connect.php';

$voltaje = htmlspecialchars($_GET["volt"]);
$cafe = htmlspecialchars($_GET["caffe"]);
$fecha= date("Y/m/d");

$sql = "INSERT INTO datos (cafe,voltaje,fecha) VALUES (". $cafe.",".$voltaje." ,'".$fecha."')";

if ($conn->query($sql) === TRUE) {
//    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

//$conn->close();

?>
<br>El nivel de bateria esta en  <?php echo $_GET["volt"]; ?><br>
La cantidad de cafe esta en: <?php echo $_GET["caffe"]; ?> <br>
La fecha es <?php echo $fecha  ?>:
</body>
</html>

