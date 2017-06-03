<?php session_start(); ?>
<html>
	<head><title>Configurations</title></head>
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
			
			$email = $_GET['email'];
			$config = $_GET['config'];
			
			
			//Ler/Receber 1 2 ou 3 a app e inserir na coluna config da tabela users
			$sql = update users set config = $config where email = $email;

			$result = $connection->exec($sql);
			

			$connection=null;
	
		?>
	</body>
</html>