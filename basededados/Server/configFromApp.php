<?php session_start(); ?>
<html>
	<head><title>Configurations from App</title></head>
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
			
			//Leitura dos dados necessarios para armazenar a configuracao actual desejada pelo utilizador 
			$email = htmlentities($_GET['email'], ENT_QUOTES);
			$configInt = htmlentities($_GET['configInt'], ENT_QUOTES);
			
			$sql = "UPDATE users SET configInt = '$configInt' WHERE email = '$email'";
			$result = $connection->query($sql);
			
			echo("<p>Configurations updated! ");
			echo("</p>");
						
			$connection = null;
		?>
	</body>
</html>