<?php session_start(); ?>
<html>

	<body>
		<?php
			$host = "db.ist.utl.pt";
			$user = "ist178094";
			$password = "kier6808";
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
			
		/*	Nao esta a funcionar com a prevencao de SQL injection 
		
			$stmt = $connection->prepare("INSERT INTO measurements VALUES (current timestamp, :temperature, :light, :moisture);");
			$stmt->bindParam(':temperature', $temperature);
			$stmt->bindParam(':light', $light);
			$stmt->bindParam(':moisture', $moisture);			
			$stmt->execute();
		*/
			$connection=null;

		?>
	</body>
</html>