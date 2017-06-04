<?php session_start(); ?>
<html>
	<head><title>Sign Up from the app</title></head>
	<body>
		<h1>Sign Up</h1>
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
			
			//Leitura dos dados de sign up do utilizador introduzidos na App
			$email = htmlentities($_GET['email'], ENT_QUOTES);
			$pass = htmlentities($_GET['pass'], ENT_QUOTES);
			$idArduino = htmlentities($_GET['idArduino'], ENT_QUOTES);
			
			$result = $connection->prepare("Select email, password from users where email=:email and password=:pass");
			$result->bindParam(':email', $email);
			$result->bindParam(':pass', $pass);
			$result->execute();
			
			if(($result->rowcount()) == 0){			
				//Se o utlizador nao constar na base de dados, insere-se o mesmo na tabela users com os dados necessarios, introduzidos através da App
				$sql = "INSERT INTO users VALUES ('$idArduino', '$email', '$pass', '2')";
				$result = $connection->exec($sql);
				
				$response['success'] = 1; //Signed in successfully!
				
			}else{
				$response['success'] = 0; //Failed to sign in!
				
			}
			echo json_encode($response);

			$connection = null;
	
		?>
	</body>
</html>