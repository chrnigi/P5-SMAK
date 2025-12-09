interface Evaluation {
    id: number;
    ply: number;
    result: string;
    reason: string;
    forced_mate: Boolean;
    pawn_eval: number;
    bestmove: string;
    ponder: string;
    move:   string;
};

interface Move {
    id:             number;
    ply_number:     number;
    move_type:      string;
    piece_moved:    string;
    piece_captured: string;
    from_square:    number;
    to_square:      number;
};

var move_array: string[] = [];
var evals: Evaluation[] = [];

function colorMoveTable(idx: number) {
    
    if (idx === 0) {

    }
    var movestable = document.getElementById("movestable");

    if (!(movestable instanceof HTMLTableElement)) {
        return;
    }

    var mtrows = movestable.rows;

    for (let i = 1; i < mtrows.length; i++) {
        var r = mtrows.item(i);
        if (r?.cells) {
            let cellcount = r.cells.length;
            for (let j = 1; j < cellcount; j++) {
                if (r.cells) {
                    r.cells.item(j)!.style.backgroundColor = "White";
                }
            } 
        }
    }

    var row = movestable.rows.item(Math.floor((idx+1)/2));
    if ((Math.floor(idx+1)/2) < 1) {
        movestable.rows.item(2)!.cells.item(1)!.style.backgroundColor = "White";
        console.error("Illegal index");
        return;
    }

    if (!(row?.cells)) {
        return;
    }
    var cellno = 1 + (idx+1)%2;

    console.log("cellno = " + cellno);

    let target_cell = row!.cells.item(cellno);
    target_cell!.style.backgroundColor = "DarkSeaGreen";

}

function populateMoveArray(mvs: string[]) {
    move_array = [];

    mvs.forEach(mv => {
        const from_str = mv.slice(0,2);
        const to_str   = mv.slice(2);
        move_array.push((from_str + "-" + to_str));
    });
}

function deleteMoves() {
    var movestable = document.getElementById("movestable");

    if (!(movestable instanceof HTMLTableElement)) {
        return;
    }

    var rows = movestable.rows;

    for (let i = rows.length-1; i >= 1; i--) {
        movestable.deleteRow(i);
    }
}

function populateMovesTable(moves: Evaluation[]) {
    var movestable = document.getElementById("movestable");
    var moveno = 0;
    if (movestable instanceof HTMLTableElement) {
        for (let i = 0; i < moves.length; i+=2) {
            moveno++;
            const move_w = moves[i].move;
            let newmove = movestable.insertRow();
            let numbercell = newmove.insertCell(0);
            let wcell = newmove.insertCell(1);
            let bcell = newmove.insertCell(2);
            
            numbercell.innerHTML = moveno.toString() + ".";
            wcell.innerHTML = move_w;

            if (!(i+1 >= moves.length)) {
                const move_b = moves[i+1].move;
                bcell.innerHTML = move_b;
            }
        }
    }
}


async function getEval(id: Number) {
    //const url: string = "http://p5webserv.head9x.dk:9090/games/eval/" + id;
    const url: string = "http://localhost:9090/games/eval/" + id;
    try {
        const response = await fetch(url);
        if (!response.ok) {
            throw new Error('Response did unnice thing: ${response.status}');
        }

        const result: Evaluation[] = await response.json();
        console.log(result);

        evals = result;

        var movesstrings: string[] = [];
        result.forEach(ev => {
            movesstrings.push(ev.move);
        });

        populateMoveArray(movesstrings);

        populateMovesTable(result);
        

    } catch (error: any) {
        console.error(error.message);
    } 
}

function clearEvalTbl() {
    let eval_tbl = document.getElementById("evaltable");
    if (eval_tbl instanceof HTMLTableElement) {
        let tbl_coll = eval_tbl.rows;
        let secondrow = tbl_coll[1];
        let cls = secondrow.cells;

        for (let i = 0; i < cls.length; i++) {
            var cell = cls[i];
            cell.innerHTML = "";
        }
    }
}

function updateEvalTbl(plyno: number) {
    let eval_tbl = document.getElementById("evaltable");

    if (eval_tbl instanceof HTMLTableElement) {
        let tbl_coll = eval_tbl.rows;
        let secondrow = tbl_coll[1];
        console.log(secondrow);
        let cls = secondrow.cells
        
        let a = evals[plyno-1];
        if (!a) {
            return;
        }

        for (let i = 0; i < cls.length; i++) {
            var cell = cls[i];
            
            switch (cell.id) {
                case "plycol":
                    cell.innerHTML = a.ply.toString();
                    break;
                case "rescol":
                    cell.innerHTML = a.result.toString();
                    break;
                case "reascol":
                    cell.innerHTML = a.reason.toString();
                    break;
                case "fccol":
                    cell.innerHTML = a.forced_mate ? "true" : "false";
                    break;
                case "evcol":
                    cell.innerHTML = a.pawn_eval.toString();
                    break;
                case "bmcol":
                    cell.innerHTML = a.bestmove;
                    break;
                case "pdcol":
                    cell.innerHTML = a.ponder;
                    break;

                default:
                    break;
            }
        }
    }
}
