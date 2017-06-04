<?php session_start(); ?>
<html>
	<head><title>Login from the app</title></head>
	<body>
		<h1>LOGIN</h1>
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
			
			//Leitura dos dados de login do utilizador introduzidos na App
			$email = htmlentities($_GET['email'], ENT_QUOTES);
			$pass = htmlentities($_GET['pass'], ENT_QUOTES);
			
			//Verificacao da existencia ou nao desses dados na base de dados
			$result = $connection->prepare("Select email, password from users where email=:email and password=:pass");
			$result->bindParam(':email', $email);
			$result->bindParam(':pass', $pass);
			$result->execute();
			
			if( ($result->rowcount())!= 0 ){
				$response['success'] = 1; // Logged in successfully!
				echo("<p></p>");
			}else{
				//Se o utilizador nao estiver registado na base de dados, entao nao  e possivel realizar o login na App
				$response['success'] = 0; //Failed to login!
			}
		
			echo json_encode($response);

			$connection = null;
	
		?>
	</body>
</html>