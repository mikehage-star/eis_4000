#pragma once

const char dashboardHtml[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>EIS-4000</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
html{-webkit-text-size-adjust:100%}
body{background:#0d1117;color:#c9d1d9;font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,Helvetica,Arial,sans-serif;height:100vh;display:flex;flex-direction:column;overflow:hidden}
header{display:flex;align-items:center;justify-content:space-between;padding:6px 14px;background:#161b22;border-bottom:1px solid #21262d;flex-shrink:0}
header h1{font-size:.95rem;font-weight:700;color:#58a6ff;letter-spacing:1px}
#status{font-size:.75rem;display:flex;align-items:center;gap:6px;color:#8b949e}
.dot{width:8px;height:8px;border-radius:50%;display:inline-block;transition:background .3s}
.dot.ok{background:#3fb950}.dot.err{background:#f85149}.dot.stale{background:#d29922}
main{flex:1;display:flex;flex-direction:column;gap:8px;padding:8px 12px;min-height:0;width:100%}
.charts-row{display:grid;grid-template-columns:1fr 1fr;gap:10px;flex:1;min-height:0}
.chart-wrap{background:#161b22;border:1px solid #21262d;border-radius:8px;padding:8px 10px 6px;display:flex;flex-direction:column;min-height:0;overflow:hidden}
.chart-wrap h2{font-size:.65rem;text-transform:uppercase;letter-spacing:1.2px;color:#8b949e;margin-bottom:4px;font-weight:600;flex-shrink:0}
.chart-wrap svg{flex:1;width:100%;min-height:0;display:block}
.chart-legend{display:flex;gap:10px;margin-top:4px;font-size:.6rem;color:#8b949e;flex-shrink:0}
.chart-legend .leg{display:flex;align-items:center;gap:4px}
.chart-legend .sw{width:10px;height:3px;border-radius:2px;display:inline-block}
.card-strip{display:grid;grid-template-columns:repeat(11,1fr);gap:6px;flex-shrink:0}
.card{background:#161b22;border:1px solid #21262d;border-radius:6px;padding:6px 4px;text-align:center;transition:border-color .3s,box-shadow .3s;min-width:0}
.card .label{font-size:.55rem;text-transform:uppercase;letter-spacing:.5px;color:#8b949e;margin-bottom:2px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
.card .value{font-size:1.05rem;font-weight:700;font-variant-numeric:tabular-nums;line-height:1.2;transition:color .3s}
.card .unit{font-size:.5rem;color:#484f58;margin-top:1px}
.card.ok .value{color:#3fb950}
.card.caution .value{color:#d29922}
.card.danger .value{color:#f85149}
.card.info .value{color:#58a6ff}
.card.neutral .value{color:#c9d1d9}
.card.danger{border-color:#f8514944;box-shadow:0 0 8px #f8514922}
.card.caution{border-color:#d2992244}
#noData{display:flex;align-items:center;justify-content:center;flex:1;color:#484f58;font-size:1rem}
footer{display:flex;justify-content:space-between;padding:4px 14px;background:#161b22;border-top:1px solid #21262d;font-size:.6rem;color:#484f58;flex-shrink:0;gap:8px;flex-wrap:nowrap}
@media(max-width:800px){
.charts-row{grid-template-columns:1fr}
.card-strip{grid-template-columns:repeat(6,1fr)}
body{overflow:auto;height:auto}
}
</style>
</head>
<body>
<header>
<h1>EIS-4000</h1>
<div id="status"><span class="dot err" id="dot"></span><span id="stxt">Connecting&hellip;</span></div>
</header>
<div id="noData" style="display:none">Waiting for EIS data&hellip;</div>
<main id="dash">
<div class="charts-row">
<div class="chart-wrap">
<h2>Cylinder Head Temp (&deg;F)</h2>
<svg id="svg-cht" viewBox="0 0 420 140" preserveAspectRatio="none"></svg>
<div class="chart-legend">
<span class="leg"><span class="sw" style="background:#58a6ff"></span>1</span>
<span class="leg"><span class="sw" style="background:#3fb950"></span>2</span>
<span class="leg"><span class="sw" style="background:#f0883e"></span>3</span>
<span class="leg"><span class="sw" style="background:#bc8cff"></span>4</span>
</div>
</div>
<div class="chart-wrap">
<h2>Exhaust Gas Temp (&deg;F)</h2>
<svg id="svg-egt" viewBox="0 0 420 140" preserveAspectRatio="none"></svg>
<div class="chart-legend">
<span class="leg"><span class="sw" style="background:#58a6ff"></span>1</span>
<span class="leg"><span class="sw" style="background:#3fb950"></span>2</span>
<span class="leg"><span class="sw" style="background:#f0883e"></span>3</span>
<span class="leg"><span class="sw" style="background:#bc8cff"></span>4</span>
</div>
</div>
</div>
<div class="card-strip">
<div class="card neutral" id="c-tach"><div class="label">RPM</div><div class="value">--</div><div class="unit">tach</div></div>
<div class="card neutral" id="c-ias"><div class="label">IAS</div><div class="value">--</div><div class="unit">KTS</div></div>
<div class="card neutral" id="c-alt"><div class="label">ALT</div><div class="value">--</div><div class="unit">FT</div></div>
<div class="card neutral" id="c-oilt"><div class="label">OIL T</div><div class="value">--</div><div class="unit">&deg;F</div></div>
<div class="card neutral" id="c-oilp"><div class="label">OIL P</div><div class="value">--</div><div class="unit">PSI</div></div>
<div class="card neutral" id="c-volt"><div class="label">VOLT</div><div class="value">--</div><div class="unit">V</div></div>
<div class="card neutral" id="c-ff"><div class="label">FUEL F</div><div class="value">--</div><div class="unit">GPM</div></div>
<div class="card neutral" id="c-fq"><div class="label">FUEL Q</div><div class="value">--</div><div class="unit">GAL</div></div>
<div class="card neutral" id="c-hrs"><div class="label">HRS</div><div class="value">--</div><div class="unit">TAS</div></div>
<div class="card neutral" id="c-baro"><div class="label">BARO</div><div class="value">--</div><div class="unit">inHg</div></div>
<div class="card neutral" id="c-vs"><div class="label">V/S</div><div class="value">--</div><div class="unit">FPM</div></div>
</div>
</main>
<footer>
<span id="ft">Flight: --:--:--</span>
<span id="fc">Frames: 0</span>
<span id="ec">Err: 0</span>
<span id="fps">-- fps</span>
</footer>
<script>
var Z={
rpm:[[0,2800,'ok'],[2800,2900,'caution'],[2900,9e5,'danger']],
cht:[[0,380,'ok'],[380,400,'caution'],[400,9e5,'danger']],
egt:[[0,1400,'ok'],[1400,1550,'caution'],[1550,9e5,'danger']],
oilt:[[0,120,'caution'],[120,230,'ok'],[230,250,'caution'],[250,9e5,'danger']],
oilp:[[0,30,'danger'],[30,40,'caution'],[40,100,'ok'],[100,9e5,'danger']],
volt:[[0,12,'danger'],[12,13,'caution'],[13,14.8,'ok'],[14.8,15.5,'caution'],[15.5,99,'danger']],
fq:[[0,5,'danger'],[5,10,'caution'],[10,9e5,'ok']]
};
function zv(v,z){
if(v==null||isNaN(v))return'neutral';
for(var i=0;i<z.length;i++)if(v>=z[i][0]&&v<z[i][1])return z[i][2];
return z[z.length-1][2];
}
function up(id,v,z,d){
var e=document.getElementById(id);if(!e)return;
var val=e.querySelector('.value');
var cls='card ';
if(v==null||isNaN(v)){val.textContent='--';cls+='neutral'}
else{val.textContent=d!=null?Number(v).toFixed(d):v;cls+=z?zv(v,z):'info'}
e.className=cls;
}
var MAXP=60;
var chtH=[[],[],[],[]];
var egtH=[[],[],[],[]];
var COLS=['#58a6ff','#3fb950','#f0883e','#bc8cff'];
function pushHist(arr,v){
for(var i=0;i<4;i++){
arr[i].push(v[i]!=null&&!isNaN(v[i])?v[i]:null);
if(arr[i].length>MAXP)arr[i].shift();
}
}
function drawChart(id,data,yMin,yMax,thresh){
var svg=document.getElementById(id);if(!svg)return;
var W=420,H=140,pL=32,pR=36,pT=8,pB=16;
var pW=W-pL-pR,pH=H-pT-pB;
var s='<rect x="'+pL+'" y="'+pT+'" width="'+pW+'" height="'+pH+'" fill="#0d1117" rx="4"/>';
for(var g=0;g<=4;g++){
var gy=pT+pH-(g/4)*pH;
var gv=Math.round(yMin+(g/4)*(yMax-yMin));
s+='<line x1="'+pL+'" y1="'+gy.toFixed(1)+'" x2="'+(pL+pW)+'" y2="'+gy.toFixed(1)+'" stroke="#21262d"/>';
s+='<text x="'+(pL-4)+'" y="'+(gy+3).toFixed(1)+'" text-anchor="end" fill="#484f58" font-size="8">'+gv+'</text>';
}
for(var t=0;t<thresh.length;t++){
var tv=thresh[t][0],tc=thresh[t][1];
if(tv<yMin||tv>yMax)continue;
var ty=pT+pH-((tv-yMin)/(yMax-yMin))*pH;
s+='<line x1="'+pL+'" y1="'+ty.toFixed(1)+'" x2="'+(pL+pW)+'" y2="'+ty.toFixed(1)+'" stroke="'+tc+'" stroke-width="1" stroke-dasharray="4,3" opacity=".5"/>';
}
for(var ci=0;ci<4;ci++){
var pts=data[ci];
if(pts.length<2)continue;
var off=MAXP-pts.length;
var path='';
for(var pi=0;pi<pts.length;pi++){
var v=pts[pi];
if(v==null||isNaN(v))continue;
var cv=Math.max(yMin,Math.min(yMax,v));
var px=pL+((off+pi)/(MAXP-1))*pW;
var py=pT+pH-((cv-yMin)/(yMax-yMin))*pH;
path+=(path?' ':'')+px.toFixed(1)+','+py.toFixed(1);
}
if(path){
s+='<polyline points="'+path+'" fill="none" stroke="'+COLS[ci]+'" stroke-width="1.5" stroke-linejoin="round"/>';
var lv=pts[pts.length-1];
if(lv!=null&&!isNaN(lv)){
var clv=Math.max(yMin,Math.min(yMax,lv));
var ly=pT+pH-((clv-yMin)/(yMax-yMin))*pH+3;
s+='<text x="'+(pL+pW+4)+'" y="'+ly.toFixed(1)+'" fill="'+COLS[ci]+'" font-size="9" font-weight="600">'+Math.round(lv)+'</text>';
}
}
}
svg.innerHTML=s;
}
var demo=true;
function seedArr(base,amp,n){
var a=[],v=base;
for(var i=0;i<n;i++){v+=(Math.random()-.5)*amp;a.push(Math.round(v))}
return a;
}
chtH[0]=seedArr(362,5,30);chtH[1]=seedArr(368,5,30);
chtH[2]=seedArr(374,5,30);chtH[3]=seedArr(380,5,30);
egtH[0]=seedArr(1210,25,30);egtH[1]=seedArr(1260,25,30);
egtH[2]=seedArr(1310,25,30);egtH[3]=seedArr(1350,25,30);
var dTach=2200,dIas=95,dOilT=185,dOilP=68,dVolt=14.3;
function demoUpdate(){
dTach+=Math.round((Math.random()-.5)*15);dTach=Math.max(1800,Math.min(2500,dTach));
dIas+=Math.round((Math.random()-.5)*3);dIas=Math.max(60,Math.min(140,dIas));
dOilT+=Math.round((Math.random()-.5)*2);dOilT=Math.max(160,Math.min(220,dOilT));
dOilP+=Math.round((Math.random()-.5)*2);dOilP=Math.max(55,Math.min(80,dOilP));
dVolt+=(Math.random()-.5)*.08;dVolt=Math.max(13.8,Math.min(14.8,dVolt));
for(var i=0;i<4;i++){
var c=chtH[i],e=egtH[i];
c.push(Math.round(c[c.length-1]+(Math.random()-.5)*6));
e.push(Math.round(e[e.length-1]+(Math.random()-.5)*25));
if(c.length>MAXP)c.shift();
if(e.length>MAXP)e.shift();
}
drawChart('svg-cht',chtH,200,500,[[380,'#d29922'],[400,'#f85149']]);
drawChart('svg-egt',egtH,500,1600,[[1400,'#d29922'],[1550,'#f85149']]);
up('c-tach',dTach,Z.rpm);
up('c-ias',dIas,null);
up('c-alt',4200+Math.round((Math.random()-.5)*50),null);
up('c-oilt',dOilT,Z.oilt);
up('c-oilp',dOilP,Z.oilp);
up('c-volt',dVolt.toFixed(1),Z.volt,1);
up('c-ff',(7.5+Math.random()*.4).toFixed(1),null,1);
up('c-fq',(45-Math.random()).toFixed(1),Z.fq,1);
up('c-hrs','1234.6',null);
up('c-baro',(29.9+Math.random()*.1).toFixed(2),null,2);
up('c-vs',Math.round(80+Math.random()*200),null);
document.getElementById('ft').textContent='Flight: DEMO';
document.getElementById('fc').textContent='Frames: sim';
document.getElementById('ec').textContent='Err: 0';
}
demoUpdate();
var lf=0,sp=0;
async function poll(){
try{
var r=await fetch('/api/telemetry');
var d=await r.json();
demo=false;
document.getElementById('noData').style.display='none';
document.getElementById('dash').style.display='flex';
var dot=document.getElementById('dot');
var stxt=document.getElementById('stxt');
if(d.stats.frames===lf){sp++}else{sp=0;lf=d.stats.frames}
if(sp>=3){dot.className='dot stale';stxt.textContent='STALE'}
else{dot.className='dot ok';stxt.textContent='LIVE'}
up('c-tach',d.tach,Z.rpm);
up('c-ias',d.ias,null);
up('c-alt',d.alt,null);
pushHist(chtH,d.cht);pushHist(egtH,d.egt);
drawChart('svg-cht',chtH,200,500,[[380,'#d29922'],[400,'#f85149']]);
drawChart('svg-egt',egtH,500,1600,[[1400,'#d29922'],[1550,'#f85149']]);
up('c-oilt',d.oiltemp,Z.oilt);
up('c-oilp',d.oilpress,Z.oilp);
up('c-volt',d.voltage,Z.volt,1);
up('c-ff',d.fflow,null,1);
up('c-fq',d.fqty,Z.fq,1);
up('c-hrs',d.hours,null,1);
up('c-baro',d.baro,null,2);
up('c-vs',d.vspeed,null);
document.getElementById('ft').textContent=
'Flight: '+String(d.ft_h).padStart(2,'0')+':'+String(d.ft_m).padStart(2,'0')+':'+String(d.ft_s).padStart(2,'0');
document.getElementById('fc').textContent='Frames: '+d.stats.frames;
document.getElementById('ec').textContent='Err: '+d.stats.errors;
}catch(e){
if(demo){demoUpdate();return}
document.getElementById('dot').className='dot err';
document.getElementById('stxt').textContent='OFFLINE';
}
}
setInterval(poll,2000);
poll();
</script>
</body>
</html>
)rawliteral";