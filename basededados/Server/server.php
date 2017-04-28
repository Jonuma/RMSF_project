<html>

	<body>
		<?php
			$host = "db.ist.utl.pt";
			$user = "ist18094";
			$password = "kier6808";
			$dsn = "mysql:host=$host;dbname=$user";
		
			$connection = new PDO($dsn, $user, $password, array(PDO::ATTR_ERRMODE => PDO::ERRMODE_WARNING));
			
		//ENT_QUOTES is needed if the data is being substituted into an HTML attribute, e.g.
		//echo '<input type="text" value="' . htmlentities($string, ENT_QUOTES) . '">";
		//This ensures that quotes are encoded, so they won't terminate the value="..." attribute prematurely.
			
			$temperature = htmlentities($_POST['temperature'], ENT_QUOTES);
			$light = htmlentities($_POST['light'], ENT_QUOTES);
			$moisture = htmlentities($_POST['moisture'], ENT_QUOTES);
			
			$minutes = current_timestamp;
			
			$stmt = $connection->prepare("INSER INTO measurements VALUES (:minutes, :temperature, :light, :moisture);");
			$stmt->bindParam(':temperature', $temperature);
			$stmt->bindParam(':light', $light);
			$stmt->bindParam(':moisture', $moisture);
			
			$stmt->execute();

			
			
		?>
	</body>
</html>