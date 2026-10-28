<?php
$status = (int) ($_GET['status'] ?? $argv[1] ?? 200);
http_response_code($status);
header('HTTP/1.1 200 OK');
header('Content-Type: text/html; charset=UTF-8');
header('X-Powered-By-Me: my-first-app');

ob_start();                    // capture the body instead of sending it
?>
<!DOCTYPE html>
<html>
<head>
  <title>Basic Response</title>
</head>
<body>
  <h1>Hello!</h1>
  <p>Server time: <?php echo date('H:i:s'); ?></p>
</body>
</html>
<?php
$body = ob_get_clean();
header('Content-Length: ' . strlen($body));  // strlen counts bytes, which is what you need
echo $body;