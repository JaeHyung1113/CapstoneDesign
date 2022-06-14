package com.example.capstonedesign;

import android.content.res.Resources;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

public class recyclerViewAdapter extends RecyclerView.Adapter<recyclerViewAdapter.viewHolder> {

    private String[] mDataset;
    public static class viewHolder extends RecyclerView.ViewHolder {
        public TextView textView;
        public viewHolder(View v) {
            super(v);
            textView = v.findViewById(R.id.textView);
        }
    }

    public recyclerViewAdapter(String[] colorData) {
        mDataset = colorData;
    }

    @NonNull
    @Override
    public viewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        // 뷰 생성
        View v = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.recyclerview, parent, false);

        viewHolder vh = new viewHolder(v);
        return vh;
    }

    @Override
    public void onBindViewHolder(@NonNull viewHolder holder, int position) {
        Resources getRes = holder.itemView.getContext().getResources();
        String getPac = holder.itemView.getContext().getPackageName();
        holder.textView.setText(mDataset[position]);
        holder.textView.setBackgroundColor(Color.parseColor(mDataset[position]));
    }

    @Override
    public int getItemCount() {
        return mDataset.length;
    }
}
