<?php
// --- CONFIG ---
$DB_HOST = '127.0.0.1';
$DB_NAME = 'REDACTED';
$DB_USER = 'REDACTED';
$DB_PASS = 'REDACTED';
$DB_CHARSET = 'utf8mb4';

// Query params
$gameId = isset($_GET['game']) ? (int)$_GET['game'] : 1;   // required game id
$moveNo = isset($_GET['move']) ? (int)$_GET['move'] : null; // optional: show a specific move

// --- PDO ---
$dsn = "mysql:host=$DB_HOST;dbname=$DB_NAME;charset=$DB_CHARSET";
$options = [
  PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
  PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
];
$pdo = new PDO($dsn, $DB_USER, $DB_PASS, $options);

// --- Load boardstate ---
if ($moveNo !== null) {
  $sql = "SELECT bs.moveno,
                 CAST(bs.x_placement AS UNSIGNED) AS xmask,
                 CAST(bs.o_placement AS UNSIGNED) AS omask,
                 g.result, g.gamestart
          FROM tictactoe.boardstates bs
          JOIN tictactoe.games g USING (id)
          WHERE bs.id = :gid AND bs.moveno = :mov
          LIMIT 1";
  $stmt = $pdo->prepare($sql);
  $stmt->execute([':gid'=>$gameId, ':mov'=>$moveNo]);
} else {
  $sql = "SELECT bs.moveno,
                 CAST(bs.x_placement AS UNSIGNED) AS xmask,
                 CAST(bs.o_placement AS UNSIGNED) AS omask,
                 g.result, g.gamestart
          FROM tictactoe.boardstates bs
          JOIN tictactoe.games g USING (id)
          WHERE bs.id = :gid
          ORDER BY bs.moveno DESC
          LIMIT 1";
  $stmt = $pdo->prepare($sql);
  $stmt->execute([':gid'=>$gameId]);
}

$state = $stmt->fetch();
if (!$state) {
  http_response_code(404);
  echo "No boardstate found for game #".htmlspecialchars((string)$gameId).".".($moveNo!==null?" Move $moveNo requested.":"");
  exit;
}

$xmask = (int)$state['xmask'];
$omask = (int)$state['omask'];
$moveno = (int)$state['moveno'];
$result = $state['result'] ?? 'inprogress';

// --- Helpers ---
function bitSet($mask, $i) { return (($mask >> $i) & 1) === 1; }

// Precompute winning lines for row-major bits 0..8 (top-left → bottom-right)
$wins = [
  (1<<0)|(1<<1)|(1<<2), // rows
  (1<<3)|(1<<4)|(1<<5),
  (1<<6)|(1<<7)|(1<<8),
  (1<<0)|(1<<3)|(1<<6), // cols
  (1<<1)|(1<<4)|(1<<7),
  (1<<2)|(1<<5)|(1<<8),
  (1<<0)|(1<<4)|(1<<8), // diagonals
  (1<<2)|(1<<4)|(1<<6),
];

// Determine winner mask (for highlight)
$winMask = 0;
$winner  = null;
foreach ($wins as $w) {
  if ( ($xmask & $w) === $w ) { $winner='X'; $winMask=$w; break; }
  if ( ($omask & $w) === $w ) { $winner='O'; $winMask=$w; break; }
}

// --- Build 3x3 view ---
$board = [];
for ($i=0; $i<9; $i++) {
  if (bitSet($xmask, $i))      $board[$i] = 'X';
  elseif (bitSet($omask, $i))  $board[$i] = 'O';
  else                         $board[$i] = null;
}
?>
<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>Tic-Tac-Toe — Game <?= htmlspecialchars((string)$gameId) ?> (Move <?= $moveno ?>)</title>
<style>
  :root { --size: 90px; --gap: 8px; --radius: 14px; --border: 2px; --font: 56px; }
  body { font-family: system-ui, sans-serif; background:#f6f7fb; margin:0; display:grid; place-items:center; min-height:100svh; }
  .wrap { display:flex; flex-direction:column; gap:14px; align-items:center; }
  .grid {
    display:grid; grid-template-columns:repeat(3, var(--size)); gap:var(--gap);
    background:#d9dce1; padding:var(--gap); border-radius: calc(var(--radius) + var(--gap));
    box-shadow: 0 8px 24px rgba(0,0,0,.08);
  }
  .cell {
    width:var(--size); height:var(--size); display:flex; align-items:center; justify-content:center;
    background:white; border-radius:var(--radius); border:var(--border) solid #e5e7eb;
    font-weight:800; font-size:var(--font); line-height:1; user-select:none;
  }
  .cell.x { color:#111827; }
  .cell.o { color:#ef4444; }
  .cell.empty { color:#cbd5e1; }
  .cell.win { outline:3px solid #10b981; box-shadow:0 0 0 3px #10b98133 inset; }
  .meta { font-size:14px; color:#475569; }
  .meta code { background:#eef2f7; padding:.2em .4em; border-radius:6px; }
  @media (max-width:480px){ :root{ --size:74px; --font:48px; } }
</style>
</head>
<body>
  <div class="wrap">
    <div class="grid" role="grid" aria-label="Tic Tac Toe Board">
      <?php for ($r=0; $r<3; $r++): ?>
        <?php for ($c=0; $c<3; $c++):
          $i = $r*3 + $c; // bit index
          $v = $board[$i];
          $cls = 'cell ' . ($v==='X'?'x':($v==='O'?'o':'empty'));
          if ($winner && (($winMask >> $i) & 1)) $cls .= ' win';
        ?>
          <div class="<?= $cls ?>" role="gridcell" aria-label="Row <?= $r+1 ?>, Col <?= $c+1 ?>">
            <?= $v ? htmlspecialchars($v, ENT_QUOTES) : '·' ?>
          </div>
        <?php endfor; ?>
      <?php endfor; ?>
    </div>
    <div class="meta">
      Game <b>#<?= htmlspecialchars((string)$gameId) ?></b>,
      showing <b>move <?= $moveno ?></b>
      <?php if ($moveNo===null): ?> (latest)<?php endif; ?>.
      Winner: <b><?= $winner ? $winner : '—' ?></b>.
      Change game via <code>?game=ID</code>; show a specific move via <code>?game=ID&amp;move=N</code>.
      Current game result: <b><?= htmlspecialchars((string)$result) ?></b>.
    </div>
  </div>
</body>
</html>

