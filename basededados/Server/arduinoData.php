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
		
			$response = array();
			
			//Estes dados sao extraidos do Arduino para armazenar na base de dados			
			$idArduino = $_GET['idArduino'];
			$minutes = date("Y-m-d H:i:s"); 
			$temperature = $_GET['temperature'];
			$light = $_GET['light'];
			$moisture = $_GET['moisture'];
			
			//Insercao na tabela das medidas da base de dados
			$sql = "INSERT INTO measurements VALUES ('$idArduino', '$minutes', '$temperature', '$light', '$moisture')";
			$result = $connection->exec($sql);
			
			//Enviar para o Arduino a configuracao actualizada pelo utilizador na App
			
			$sql = "SELECT configInt FROM users WHERE idArduino='$idArduino'";
			$config = $connection->query($sql);
				
			foreach($config as $row)
			{
				 $configInt = $row['configInt'];
			}
			
			$response['config'] = $configInt;
			
			echo json_encode($response);
			
			$connection = null;

		?>
	</body>
</html>