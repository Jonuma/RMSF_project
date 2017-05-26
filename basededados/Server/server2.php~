<?php session_start(); ?>
<html>

	<body>
		<?php
			$host = "db.ist.utl.pt";
			$user = "ist178799";
			$password = "lvqu4307";
			$dsn = "mysql:host=$host;dbname=$user";

			try{
				$connection = new PDO($dsn, $user, $password);
			}
			catch(PDOException $exception){
				echo("<p>Error: ");
				echo($exception->getMessage());
				echo("</p>");
				exit();
			}
			
		//ENT_QUOTES is needed if the data is being substituted into an HTML attribute, e.g.
		//echo '<input type="text" value="' . htmlentities($string, ENT_QUOTES) . '">";
		//This ensures that quotes are encoded, so they won't terminate the value="..." attribute prematurely.

			//Estes dados sao para se ir buscar ao Arduino para armazenar na base de dados
			
			//$temperature = htmlentities($_GET['temperature'], ENT_QUOTES);
			//$light = htmlentities($_GET['light'], ENT_QUOTES);
			//$moisture = htmlentities($_GET['moisture'], ENT_QUOTES);
			$minutes = date("Y-m-d h:i:sa"); 
			$temperature = $_GET['temperature'];
			$light = $_GET['light'];
			$moisture = $_GET['moisture'];
			
			$sql = "INSERT INTO measurements VALUES ('$minutes', '$temperature', '$light', '$moisture')";
			$result = $connection->exec($sql);

			/*
			$stmt = $connection->prepare("INSERT INTO measurements VALUES (:minutes, :temperature, :light, :moisture);");
			$stmt->bindParam(':temperature', $temperature);
			$stmt->bindParam(':light', $light);
			$stmt->bindParam(':moisture', $moisture);			
			$stmt->execute();
			*/
			$connection=null;

		/*
			// array for JSON response
			$response = array();

			// check for required fields
			if (isset($_POST['name']) && isset($_POST['price']) && isset($_POST['description'])) {

			$name = $_POST['name'];
			$price = $_POST['price'];
			$description = $_POST['description'];

			// include db connect class
			require_once __DIR__ . '/db_connect.php';

			// connecting to db
			$db = new DB_CONNECT();

			// mysql inserting a new row
			$result = mysql_query("INSERT INTO products(name, price, description) VALUES('$name', '$price', '$description')");

			// check if row inserted or not
			if ($result) {
			// successfully inserted into database
			$response["success"] = 1;
			$response["message"] = "Product successfully created.";

			// echoing JSON response
			echo json_encode($response);
			} else {
			// failed to insert row
			$response["success"] = 0;
			$response["message"] = "Oops! An error occurred.";

			// echoing JSON response
			echo json_encode($response);
			}
			} else {
			// required field is missing
			$response["success"] = 0;
			$response["message"] = "Required field(s) is missing";

			// echoing JSON response
			echo json_encode($response);
		*/

		?>
	</body>
</html>
