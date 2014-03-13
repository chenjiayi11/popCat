/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.taobao.popcat;

import java.util.ArrayList;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.laiwang.opensdk.LWOpenApi;
import com.laiwang.opensdk.exception.ServiceException;
import com.laiwang.opensdk.model.GameRank;
import com.laiwang.opensdk.model.Rank;
import com.laiwang.opensdk.model.RankList;
import com.laiwang.opensdk.model.UserGameInfo;

import android.os.Bundle;
import android.os.Handler;
import android.os.Handler.Callback;
import android.os.Message;
import android.widget.Toast;

public class popCat extends Cocos2dxActivity implements Callback{
	private static LWOpenApi mLWOpenApi;
	private static Handler handler;
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		mLWOpenApi = LWOpenApi.instance();
		handler = new Handler(this);
		startGame();
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// popCat should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }
    
	static {
        System.loadLibrary("popCat");
    }
    
    public static void startGame(){
    	final Message message = handler.obtainMessage();
    	new Thread(){
    		@Override
            public void run() {
    			try {
    				boolean result = mLWOpenApi.getGameService().startGame();
    				message.obj = result;
				} catch (ServiceException e) {
					e.printStackTrace();
					message.obj = e.toString();
				}
    			handler.sendMessage(message);
    		}
    	}.start();
    }
    
    public static void commitGameScore(final int score){
    	final Message message = handler.obtainMessage();
    	new Thread(){
    		@Override
			public void run() {
				Rank rank = new Rank();
				rank.setKey("score");
				rank.setValue(score);
				ArrayList<String> list = new ArrayList<String>();
				list.add(Rank.RollUps.MAX_THIS_WEEK.getValue());
				rank.setRollUps(list);
				ArrayList<Rank> ranks = new ArrayList<Rank>();
				ranks.add(rank);
				String userDate="score";
				try {
					GameRank gameRank = mLWOpenApi.getGameService().upLoadValue(ranks, userDate);
                    message.obj = gameRank;
				} catch (Exception e) {
					 e.printStackTrace();
                     message.obj = e;
				}
				handler.sendMessage(message);
			}
    	}.start();
    }
    
    public static Object[] getScoreList(){
    	RankList rankList = null;
    	try {
    		rankList = mLWOpenApi.getGameService().requestRankList("score", Rank.RollUps.MAX_THIS_WEEK, 0, 50);
        } catch (ServiceException e) {
            e.printStackTrace();
        }
    	if(rankList == null)
    		return null;
    	return rankList.getTop().toArray();
    }

	@Override
	public boolean handleMessage(Message msg) {
		if(msg != null && msg.obj != null){
			Toast.makeText(this, msg.obj.toString(), Toast.LENGTH_SHORT).show();
		}
		
		return false;
	}     
}
